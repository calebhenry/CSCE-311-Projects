#include "./SharedMemClient.h"

SharedMemClient::SharedMemClient(::pthread_t id, ::size_t index, std::vector<std::string>* lines_, std::atomic<int>* num_found_, std::vector<std::string>* search_, bool* complete_) 
  :Thread<SharedMemClient>(id, index),
  num_found(num_found_),
  lines(lines_),
  search(search_),
  writing_("writing_name"),
  reading_("shm_finished_name"), 
  barrier_("bar"),
  available_lines("av"),
  print_lock("pr")
  {

  shm_name_ = "letsgoo";
  writing_.Open();
  reading_.Open();
  barrier_.Open();
  available_lines.Open();
  print_lock.Open();
    
}

void SharedMemClient::runClient(int argc, char *argv[]){
    int shm_fd = ::shm_open(shm_name_.c_str(), O_RDWR, 0);
    if (shm_fd < 0) {
        std::cerr << "Producer::Produce ::shm_open: " << ::strerror(errno)
         << std::endl;
         //return errno;
    }

  // get copy of mapped mem
  SharedMemoryStore<kSharedMemSize>* store
    = static_cast<SharedMemoryStore<kSharedMemSize>*>(
      ::mmap(nullptr,
             kSharedMemSize,
             PROT_READ | PROT_WRITE,
             MAP_SHARED,
             shm_fd,
             0
      )
    );
  if (store == MAP_FAILED) {
    std::cerr << "Producer::Produce ::mmap: " << ::strerror(errno)
      << std::endl;
  }
  // Parse input
  std::string msg = argv[1];

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

  // copy string msg into shared memory via strncpy ('n' for bounded copy)
  strncpy(store->buffer, msg.c_str(), store->buffer_size);
  
  // signal consumer
  writing_.Up();
  barrier_.Up();
  while (true)
  {
    reading_.Down();
    barrier_.Down();  // block until occupied signal
    std::string buff(store->buffer);
    while(buff.find('\n') != std::string::npos){
      lines->push_back(buff.substr(0, buff.find('\n')));
      available_lines.Up();
      buff = buff.substr(buff.find('\n')+1);
    }
    if(buff.find(kEoT) != std::string::npos){
      break;
    }
    barrier_.Up();
    writing_.Up();
  }
  for(size_t i = 0; i < lines->size(); i++){
    std::cout << lines->at(i) << std::endl;
  }
}

void* SharedMemClient::Execute(void* ptr) {
  auto client = static_cast<::SharedMemClient *>(ptr);
  int index = 0;
  while(true){
    client->available_lines.Down();
    std::string line = (client->lines)->at(index);
    if (client->search->at(0) == "x") {
      bool add = true;
      for (size_t i = 2; i < client->search->size(); i++) {
        if (line.find(client->search->at(i)) == std::string::npos) {
          add = false;
        }
      }
      if (add) {
        *(client->num_found) += 1;
        line = std::to_string(*client->num_found) + "\t" + line + " done by thread " + std::to_string(client->id());
        client->PrintThreaded(line);
        std::cout << std::endl;
      }
    } else if (client->search->at(0) == "+") {
      bool add = false;
      for (size_t i = 2; i < client->search->size(); i++) {
        if (line.find(client->search->at(i)) != std::string::npos) {
          add = true;
        }
      }
      if (add) {
        *(client->num_found) += 1;
        line = std::to_string(*client->num_found) + "\t" + line + " done by thread " + std::to_string(client->id());
        client->PrintThreaded(line);
        std::cout << std::endl;
      }
    } else {
      if (line.find(client->search->at(1)) != std::string::npos) {
        *(client->num_found) += 1;
        line = std::to_string(*client->num_found) + "\t" + line;
        client->PrintThreaded(line);
        std::cout << std::endl;
      }
    }

    index++;
  }
  return ptr;
}

void SharedMemClient::PrintThreaded(const std::string& msg) {
  print_lock.Down();
  std::cout << msg;
  print_lock.Up();
}