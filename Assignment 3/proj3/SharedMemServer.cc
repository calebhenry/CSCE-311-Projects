#include "./SharedMemServer.h"

SharedMemServer::SharedMemServer() 
    :writing_("writing_name"),
    reading_("shm_finished_name"),
    barrier_("bar"),
    available_lines("av"),
    print_lock("pr") {

    shm_name_ = "letsgoo";

    //Create Barrier
    writing_.Create(0);
    writing_.Open();

    reading_.Create(0);
    reading_.Open();

    barrier_.Create(0);
    barrier_.Open();

    available_lines.Create(0);
    available_lines.Open();
    
    print_lock.Create(1);
    print_lock.Open();
}

void SharedMemServer::runServer() {
  // write any logs to file
  SharedMemoryStore<kSharedMemSize>* store;
  const char kEoT = static_cast<char>(3);
  std::vector<std::string> file;
  std::ifstream file_read;
  std::string line;
  std::string msg;

  size_t num_access = 0;
  

  while (true) {
    writing_.Down();  // block until occupied signal
    barrier_.Down();
    if(num_access == 0) {
        int shm_fd = ::shm_open(shm_name_.c_str(), O_RDWR, 0);
        if (shm_fd < 0) {
            std::cerr << "Producer::Produce ::shm_open: " << ::strerror(errno)
            << std::endl;
            //return errno;
        }

        // get copy of mapped mem
        store = static_cast<SharedMemoryStore<kSharedMemSize>*>(
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
        file.clear();
        file_read.open(store->buffer);
        if (file_read.is_open()) {
            while (std::getline(file_read, line)) {
                file.push_back(line);
            }
            file_read.close();
        }
    }
    if(file.size() > ((num_access+1)*4)){
        msg = file[num_access*4] + "\n" + file[(num_access*4)+1] + "\n" + file[(num_access*4)+2] + "\n" + file[(num_access*4)+3] + "\n";
    } else {
        msg.clear();
        for(size_t i = (num_access)*4; i < file.size(); i++) {
            msg += file[i] + "\n";
        }
        msg += kEoT;
        num_access = -1;
    }
    strncpy(store->buffer, msg.c_str(), store->buffer_size);
    std::cout << msg;
    barrier_.Up();
    reading_.Up(); // return semaphore
    
    num_access++;
    
  }
}