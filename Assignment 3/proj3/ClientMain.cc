#include "./SharedMemClient.h"

int main(int argc, char *argv[]) {
    std::vector<std::string> search(0);
    std::vector<std::string> lines(0);
    std::vector<std::string> searched(0);
    SharedMemClient main_client(pthread_t(), 5, &lines, &search, &searched);
    main_client.runClient(argc, argv);
}