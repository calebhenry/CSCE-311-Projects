
#ifndef INC_SHAREDMEMCLIENT_H_
#define INC_SHAREDMEMCLIENT_H_

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
#include <atomic>

#include "./NamedSemaphore.h"
#include "./store.h"
#include "./Thread.h"

class SharedMemClient : public Thread<SharedMemClient> {
 public:
    SharedMemClient(::pthread_t, ::size_t,
      std::vector<std::string>* lines, std::vector<std::string>* search,
      std::vector<std::string>* searched);
    void runClient(int argc, char *argv[]);
    static void* Execute(void* ptr);

 private:
    std::string ProcessString(int argc, char *argv[]);
    void AddVector(const std::string& msg);

    static const std::size_t kBufferSize = 1024;
    static const std::size_t kSharedMemSize =
      SharedMemoryStoreSizeInPages(kBufferSize);

    std::vector<std::string>* lines;
    std::vector<std::string>* search;
    std::vector<std::string>* searched;

    wrappers::NamedSemaphore writing_;  // shared memory log mutex
    wrappers::NamedSemaphore reading_;
    wrappers::NamedSemaphore barrier_;
    wrappers::NamedSemaphore print_lock;

    std::string shm_name_ = "memshare";  // shared memory name

    SharedMemoryStore<kSharedMemSize> *store_;
};

#endif  // INC_SHAREDMEMCLIENT_H_
