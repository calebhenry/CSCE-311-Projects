File 1: ClientMain.cc
    Purpose: Takes a users input and feeds it into a client

File 2: ServerMain.cc
    Purpose: Takes a users input and feeds it into a server

File 3: DomainSocketServer.h
    Purpose: The header file for DomainSocketServer.cc

File 4: DomainSocketServer.cc
    Purpose: Holds the Run function (along with its helper functions) 
    which connects and shares information with its client counterpart

File 5: DomainSocketClient.h
    Purpose: The header file for DomainSocketClient.cc

File 6: DomainSocketClient.cc
    Purpose: Holds the Run function (along with its helper functions) 
    which connects and shares information with its server counterpart

File 7: UnixDomainSocket.h
    Purpose: The header file for UnixDomainSocket.cc

File 8: UnixDomainSocket.cc
    Purpose: Creates the struct for the domain socket

File 9: makefile
    Purpose: Creates the client executable when given the command make text-client, 
    creates the server executable when given the command make text-server. and 
    cleans out intermediary files when given the command make clean

File 10: bankloan1.csv
    Purpose: Sample file for the server to parse

File 11: bankloan2.csv
    Purpose: Sample file for the server to parse