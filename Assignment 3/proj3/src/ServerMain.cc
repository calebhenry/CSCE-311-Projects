/**
 * @file ServerMain.cc
 * @author Caleb Henry
 * @brief Starts an instance of the server
 * @version 0.1
 * @date 2023-04-03
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "../inc/SharedMemServer.h"

int main(int argc, char *argv[]) {
    SharedMemServer server;
    server.runServer();
}
