text-client : ./ClientMain.cc ./NamedSemaphore.cc ./SharedMemClient.cc
	g++ -std=c++17 -Wall -pedantic -pthread ./ClientMain.cc ./NamedSemaphore.cc ./SharedMemClient.cc -lrt -o text-client

text-server: ./ServerMain.cc ./NamedSemaphore.cc ./SharedMemServer.cc
	g++ -std=c++17 -Wall -pedantic -pthread ./ServerMain.cc ./NamedSemaphore.cc ./SharedMemServer.cc -lrt -o text-server
clean:
	rm -f bin/* text-client text-server