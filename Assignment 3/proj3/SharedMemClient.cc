#include "./SharedMemClient.h"

SharedMemClient::SharedMemClient(::pthread_t id, ::size_t index) 
  :Thread<SharedMemClient>(id, index),
  writing_("writing_name"),
  reading_("shm_finished_name"), 
  barrier_("bar"),
  available_lines("av"),
  print_lock("pr") {

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
  search = new std::vector<std::string>;
  lines = new std::vector<std::string>;
  num_found = new int(0);
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
}

void* SharedMemClient::Execute(void* ptr) {
  auto client = static_cast<::SharedMemClient *>(ptr);
  
  //std::string line = (*client->lines)[*client->num_found];
  //(*client->num_found)++;
  client->PrintThreaded("hey");
  return ptr;
}

void SharedMemClient::PrintThreaded(const std::string& msg) {
  std::clog << "here";
  print_lock.Down();
  std::clog << "here";
  std::cout << msg;
  print_lock.Up();
}