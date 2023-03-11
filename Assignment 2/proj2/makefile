text-client : ./ClientMain.cc ./UnixDomainSocket.cc ./DomainSocketClient.cc
	g++ -std=c++17 -Wall -pedantic ./ClientMain.cc ./UnixDomainSocket.cc ./DomainSocketClient.cc -o text-client

text-server: ./ServerMain.cc ./UnixDomainSocket.cc ./DomainSocketServer.cc
	g++ -std=c++17 -Wall -pedantic ./ServerMain.cc ./UnixDomainSocket.cc ./DomainSocketServer.cc -o text-server
clean:
	rm -f bin/* text-client text-server