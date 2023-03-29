#include "./SharedMemClient.h"

int main(int argc, char *argv[]) {
    std::vector<std::string> search(0);
    std::vector<std::string> lines(0);
    std::atomic<int> num_found(0);
    bool complete = true;
    SharedMemClient main_client(pthread_t(), 5, &lines, &num_found, &search, &complete);
    main_client.runClient(argc, argv);
    std::vector<SharedMemClient> clients;
    for(int i = 0; i < 4; i++) {
        clients.push_back(::SharedMemClient(pthread_t(), i, &lines, &num_found, &search, &complete));
    }
    for(auto& client : clients){
        pthread_create(&client.thread_id(), nullptr, ::SharedMemClient::Execute, static_cast<void *>(&client));
    }
    for (auto& client : clients){
        pthread_join(client.thread_id(), nullptr);
    }
}