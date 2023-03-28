#ifndef SHAREDMEMCLIENT_H_
#define SHAREDMEMCLIENT_H_

#include "./NamedSemaphore.h"
#include "./store.h"
#include "./Thread.h"

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

class SharedMemClient : public Thread<SharedMemClient> {
    public:
     SharedMemClient(::pthread_t, ::size_t);
     void runClient(int argc, char *argv[]);
     static void* Execute(void* ptr);
    private:
     std::string ProcessString(int argc, char *argv[]);
     void PrintThreaded(const std::string& msg);
     static const std::size_t kBufferSize = 1024;
     static const std::size_t kSharedMemSize =
       SharedMemoryStoreSizeInPages(kBufferSize);

     std::string shm_name_ = "memshare";  // shared memory name
     wrappers::NamedSemaphore writing_;  // shared memory log mutex
     wrappers::NamedSemaphore reading_;
     wrappers::NamedSemaphore barrier_;
     wrappers::NamedSemaphore available_lines;
     wrappers::NamedSemaphore print_lock;

     std::vector<std::string>* search;
     std::vector<std::string>* lines;
     int* num_found;
};

#endif // SHAREDMEMCLIENT_H_