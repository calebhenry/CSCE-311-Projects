#include "../inc/DomainSocketClient.h"
#include <string>
#include <iostream>


int main(int argc, char *argv[]){
    DomainSocketClient dsc(argv[1]);
    dsc.Run(argc, argv);
}