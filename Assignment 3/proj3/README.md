File 1: ClientMain.cc
    Purpose: Takes a users input and feeds it into a client

File 2: ServerMain.cc
    Purpose: Starts up the server

File 3: SharedMemServer.h
    Purpose: The header file for SharedMemServer.cc

File 4: SharedMemServer.cc
    Purpose: Holds the Run function (along with its helper functions) 
    which connects and shares information with its client counterpart

File 5: SharedMemClient.h
    Purpose: The header file for SharedMemClient.cc

File 6: SharedMemClient.cc
    Purpose: Holds the Run function (along with its helper functions) 
    which connects and shares information with its server counterpart.
    It also uses threads to search the information

File 7: NamedSemaphore.h
    Purpose: The header file for NamedSemaphor.cc

File 8: NamedSemaphor.cc
    Purpose: Creates a wrapper class that allows for named semaphore construction

File 9: store.h
    Purpose: Creates the struct that store the data in shared memory

File 10: Thread.h
    Purpose: A template class for threads

File 11: makefile
    Purpose: Creates the client executable when given the command make text-client, 
    creates the server executable when given the command make text-server. and 
    cleans out intermediary files when given the command make clean

File 12: bankloan1.csv
    Purpose: Sample file for the server to parse

File 13: bankloan2.csv
    Purpose: Sample file for the server to parse