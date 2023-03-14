/**
 * @file DomainSocketServer.h
 * @author Caleb Henry
 * @brief Holds the RunServer function (along with its helper functions) 
 * which connects and shares information with its client counterpart
 * @version 0.1
 * @date 2023-02-24
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef INC_DOMAINSOCKETSERVER_H_
#define INC_DOMAINSOCKETSERVER_H_

#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include <cassert>
#include <cerrno>
#include <cstddef>
#include <cstdlib>
#include <cstring>

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include "UnixDomainSocket.h"

class DomainSocketServer : public UnixDomainSocket {
 public:
  using ::UnixDomainSocket::UnixDomainSocket;

  void UnixDomainSocket::RunServer() const;
 private:
  int CreateSocket() const;

  std::vector<std::string> RunSearch(std::string str) const;

  std::vector<std::string> Explode(std::string str) const;

  void SendData(std::vector<std::string> lines, int sock_fd) const;
};

#endif  // INC_DOMAINSOCKETSERVER_H_
