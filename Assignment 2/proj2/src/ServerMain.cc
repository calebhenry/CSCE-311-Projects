/**
 * @file ServerMain.cc
 * @author Caleb Henry
 * @brief Takes a users input and feeds it into a server
 * @version 0.1
 * @date 2023-03-13
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "../inc/DomainSocketServer.h"

int main(int argc, char *argv[]) {
    DomainSocketServer dsc(argv[1]);
    dsc.RunServer();
}
