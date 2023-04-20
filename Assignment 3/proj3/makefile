csv-client : ./src/ClientMain.cc ./src/NamedSemaphore.cc ./src/SharedMemClient.cc
	g++ -std=c++17 -Wall -pedantic -pthread ./src/ClientMain.cc ./src/NamedSemaphore.cc ./src/SharedMemClient.cc -lrt -o csv-client

csv-server: ./src/ServerMain.cc ./src/NamedSemaphore.cc ./src/SharedMemServer.cc
	g++ -std=c++17 -Wall -pedantic -pthread ./src/ServerMain.cc ./src/NamedSemaphore.cc ./src/SharedMemServer.cc -lrt -o csv-server
clean:
	rm -f bin/* csv-client csv-server