
text-client : ./src/ClientMain.cc ./src/UnixDomainSocket.cc ./src/DomainSocketClient.cc
	g++ -std=c++17 -Wall -pedantic ./src/ClientMain.cc ./src/UnixDomainSocket.cc ./src/DomainSocketClient.cc -o text-client

text-server: ./src/ServerMain.cc ./src/UnixDomainSocket.cc ./src/DomainSocketServer.cc
	g++ -std=c++17 -Wall -pedantic ./src/ServerMain.cc ./src/UnixDomainSocket.cc ./src/DomainSocketServer.cc -o text-server
clean:
	rm -f bin/* text-client text-server