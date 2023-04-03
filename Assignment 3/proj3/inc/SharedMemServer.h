#ifndef SHAREDMEMSERVER_H_
#define SHAREDMEMSERVER_H_

#include "./NamedSemaphore.h"
#include "./store.h"

#include <sys/mman.h>  // POSIX shared memory map
#include <sys/unistd.h>  // Unix standard header (ftruncate)

#include <cassert>
#include <cerrno>
#include <csignal>
#include <cstddef>  // size_t

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class SharedMemServer {
    public:
     SharedMemServer();
     void runServer();
    private:
     static const std::size_t kBufferSize = 1024;
     static const std::size_t kSharedMemSize =
      SharedMemoryStoreSizeInPages(kBufferSize);

      std::string shm_name_;  // shared memory name
      wrappers::NamedSemaphore writing_;  // shared memory barrier mutex
      wrappers::NamedSemaphore reading_;
      wrappers::NamedSemaphore barrier_;
      wrappers::NamedSemaphore print_lock;

      SharedMemoryStore<kSharedMemSize> *store_;
};

#endif // SHAREDMEMSERVER_H_