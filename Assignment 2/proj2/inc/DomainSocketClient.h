/**
 * @file DomainSocketClient.h
 * @author Caleb Henry
 * @brief Holds the RunClient function (along with its helper functions) 
 * which connects and shares information with its server counterpart
 * @version 0.1
 * @date 2023-02-24
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef INC_DOMAINSOCKETCLIENT_H_
#define INC_DOMAINSOCKETCLIENT_H_

#include <sys/socket.h>  // Unix header for sockets, using socket
#include <sys/un.h>  // defns for Unix domain sockets, using struct sockaddr_un
#include <unistd.h>  // Unix standard header, using close

#include <cassert>  // using assert
#include <cerrno>  // using errno
#include <cstddef>  // using size_t
#include <cstdlib>  // exit
#include <cstring>  // using strncpy, strerror

#include <string>
#include <iostream>
#include <vector>

#include "UnixDomainSocket.h"

class DomainSocketClient : public UnixDomainSocket {
 public:
  using UnixDomainSocket::UnixDomainSocket;

  void RunClient(int argc, char *argv[]);
 private:
  void MakeSocket();
  std::string ProcessString(int argc, char *argv[]);
  void ReceiveData(int);
};
#endif  // INC_DOMAINSOCKETCLIENT_H_
