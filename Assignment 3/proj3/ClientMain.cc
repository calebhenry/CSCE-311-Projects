#include "./SharedMemClient.h"

int main(int argc, char *argv[]) {
    SharedMemClient main_client(pthread_t(), 5);
    main_client.runClient(argc, argv);
    std::vector<SharedMemClient> clients;
    for(int i = 0; i < 4; i++) {
        clients.push_back(::SharedMemClient(pthread_t(), i));
    }
    for(auto& client : clients){
        pthread_create(&client.thread_id(), nullptr, ::SharedMemClient::Execute, static_cast<void *>(&client));
    }
    for (auto& client : clients){
        pthread_join(client.thread_id(), nullptr);
    }
}