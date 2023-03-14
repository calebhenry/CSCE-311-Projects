/**
 * @file ClientMain.cc
 * @author Caleb Henry
 * @brief Takes a users input and feeds it into a client
 * @version 0.1
 * @date 2023-03-13
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "../inc/DomainSocketClient.h"

int main(int argc, char *argv[]) {
    DomainSocketClient dsc(argv[1]);
    dsc.RunClient(argc, argv);
}
