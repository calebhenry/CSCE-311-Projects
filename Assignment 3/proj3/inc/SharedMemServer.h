/**
 * @file SharedMemServer.h
 * @author Caleb Henry
 * @brief Fetches data requested by the client then returns it by shared memory
 * @version 0.1
 * @date 2023-04-03
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef INC_SHAREDMEMSERVER_H_
#define INC_SHAREDMEMSERVER_H_

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

#include "./NamedSemaphore.h"
#include "./store.h"

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

#endif  // INC_SHAREDMEMSERVER_H_
