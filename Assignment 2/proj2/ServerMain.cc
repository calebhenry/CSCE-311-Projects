#include "./DomainSocketServer.h"
#include <string>
using std::string;

int main(int argc, char *argv[]){
    DomainSocketServer dsc(argv[1]);
    dsc.RunServer();
}