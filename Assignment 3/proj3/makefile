text-client : ./src/ClientMain.cc ./src/NamedSemaphore.cc ./src/SharedMemClient.cc
	g++ -std=c++17 -Wall -pedantic -pthread ./src/ClientMain.cc ./src/NamedSemaphore.cc ./src/SharedMemClient.cc -lrt -o text-client

text-server: ./src/ServerMain.cc ./src/NamedSemaphore.cc ./src/SharedMemServer.cc
	g++ -std=c++17 -Wall -pedantic -pthread ./src/ServerMain.cc ./src/NamedSemaphore.cc ./src/SharedMemServer.cc -lrt -o text-server
clean:
	rm -f bin/* text-client text-server