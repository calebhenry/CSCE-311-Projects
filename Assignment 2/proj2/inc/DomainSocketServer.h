/**
 * @file DomainSocketServer.h
 * @author Caleb Henry
 * @brief 
 * @version 0.1
 * @date 2023-02-24
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef DOMAINSOCKETSERVER_H
#define DOMAINSOCKETSERVER_H

#include <sys/socket.h>  // Unix header for sockets, using socket
#include <sys/un.h>  // defns for Unix domain sockets, using struct sockaddr_un
#include <unistd.h>  // Unix standard header, using close

#include <cassert>  // using assert
#include <cerrno>  // using errno
#include <cstddef>  // using size_t
#include <cstdlib>  // exit
#include <cstring>  // using strncpy, strerror

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include "UnixDomainSocket.h"

class DomainSocketServer : public UnixDomainSocket {
 public:
  using ::UnixDomainSocket::UnixDomainSocket;

  void RunServer() const;
 private:
 
  void CreateSocket(int &sock_fd, int &client_req_sock_fd) const;
  std::vector<std::string> RunSearch(std::string str) const;
  std::vector<std::string> Explode(std::string str) const;
  void SendData(std::vector<std::string> lines, int sock_fd) const;
};

#endif // DOMAINSOCKETSERVER_H