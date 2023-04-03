#include "../inc/SharedMemClient.h"

SharedMemClient::SharedMemClient(::pthread_t id, ::size_t index,
  std::vector<std::string>* lines_,
  std::vector<std::string>* search_, std::vector<std::string>* searched_)
  : Thread<SharedMemClient>(id, index),
    lines(lines_),
    search(search_),
    searched(searched_),
    writing_("writing_name"),
    reading_("shm_finished_name"),
    barrier_("bar"),
    print_lock("pr") {
  // Step 1
  shm_name_ = "shm_addr";
  shm_unlink(shm_name_.c_str());
  int shm_fd = ::shm_open(shm_name_.c_str(), O_CREAT | O_EXCL | O_RDWR, 0660);
  if (shm_fd < 0) {
      std::cerr << "Consumer::Cnsumer --- ::shm_open(" << shm_name_ << ") --- "
        <<  ::strerror(errno) << std::endl;
        ::exit(errno);
  }
  // set size of shared memory with file descriptor
  if (::ftruncate(shm_fd, kSharedMemSize) < 0) {
      std::cerr << ::strerror(errno) << std::endl;
      ::exit(errno);
  }

  // Get a copy of the memory
  const int kProt = PROT_READ | PROT_WRITE;
  store_ = static_cast<SharedMemoryStore<kSharedMemSize>*>(
      ::mmap(nullptr, kSharedMemSize, kProt, MAP_SHARED, shm_fd, 0));

  if (store_ == MAP_FAILED) {
      std::cerr << ::strerror(errno) << std::endl;
      ::exit(errno);
  }
  store_->buffer_size = kBufferSize;

  writing_.Open();
  reading_.Open();
  barrier_.Open();
  print_lock.Open();
}

void SharedMemClient::runClient(int argc, char *argv[]) {
  // Parse input
  std::string msg = argv[1];
  int index(0);

  if (argc == 3) {
    search->push_back("-");
    search->push_back(argv[2]);
  } else {
    search->push_back(argv[3]);
    for (int i = 2; i < argc; i++) {
      if (i % 2 == 0) {
        search->push_back(argv[i]);
      } else {
        if (argv[3] != argv[i]) {
          std::cerr << "Mixed boolean operators not presently supported"
            << std::endl;
          exit(2);
        }
      }
    }
  }

  const char kEoT = static_cast<char>(3);

  // Step 2
  strncpy(store_->buffer, msg.c_str(), store_->buffer_size);

  // Step 3
  writing_.Up();
  barrier_.Up();
  while (true) {
    reading_.Down();
    barrier_.Down();  // block until occupied signal
    std::string buff(store_->buffer);
    if (index == 0 && buff[0] == kEoT) {
      std::cerr << "INVALID FILE"
            << std::endl;
      shm_unlink(shm_name_.c_str());
      exit(2);
    }
    while (buff.find('\n') != std::string::npos) {
      lines->push_back(buff.substr(0, buff.find('\n')));
      buff = buff.substr(buff.find('\n')+1);
    }
    if (buff.find(kEoT) != std::string::npos) {
      break;
    }
    barrier_.Up();
    writing_.Up();
    index++;
  }

  // Step 4
  std::vector<SharedMemClient> clients;
  for (int i = 0; i < 4; i++) {
      clients.push_back(::SharedMemClient(pthread_t(), i,
        lines, search, searched));
  }
  for (auto& client : clients) {
      pthread_create(&client.thread_id(), nullptr, ::SharedMemClient::Execute,
        static_cast<void *>(&client));
  }
  for (auto& client : clients) {
      pthread_join(client.thread_id(), nullptr);
  }
  // Step 5
  for (std::string line : *searched) {
    std::cout << line << std::endl;
  }
  // Step 6
  shm_unlink(shm_name_.c_str());
  // Step 7
  exit(0);
}

void* SharedMemClient::Execute(void* ptr) {
  auto client = static_cast<::SharedMemClient *>(ptr);
  std::vector<std::string> thread_lines(0);

  // Divide up lines
  size_t num_each = ((client->lines)->size())/4;
  for (size_t i = client->id() * num_each;
      i < (client->id()+1) * num_each; i++) {
    thread_lines.push_back((client->lines)->at(i));
  }
  if (client->id() == 3) {
    for (size_t i = (client->id()+1) * num_each;
        i < (client->lines->size()); i++) {
      thread_lines.push_back((client->lines)->at(i));
    }
  }

  // Parse lines
  for (auto line : thread_lines) {
    if (client->search->at(0) == "x") {
      bool add = true;
      for (size_t i = 1; i < client->search->size(); i++) {
        if (line.find(client->search->at(i)) == std::string::npos) {
          add = false;
        }
      }
      if (add) {
        client->AddVector(line);
      }
    } else if (client->search->at(0) == "+") {
      bool add = false;
      for (size_t i = 1; i < client->search->size(); i++) {
        if (line.find(client->search->at(i)) != std::string::npos) {
          add = true;
        }
      }
      if (add) {
        client->AddVector(line);
      }
    } else {
      if (line.find(client->search->at(1)) != std::string::npos) {
        client->AddVector(line);
      }
    }
  }
  return ptr;
}

void SharedMemClient::AddVector(const std::string& msg) {
  print_lock.Down();
  std::string line = std::to_string(searched->size()+1) + "\t" + msg;
  searched->push_back(line);
  print_lock.Up();
}
