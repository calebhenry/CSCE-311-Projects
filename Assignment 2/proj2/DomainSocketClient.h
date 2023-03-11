/**
 * @file DomainSocketClient.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-02-24
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef DOMAINSOCKETCLIENT_H
#define DOMAINSOCKETCLIENT_H

#include <sys/socket.h>  // Unix header for sockets, using socket
#include <sys/un.h>  // defns for Unix domain sockets, using struct sockaddr_un
#include <unistd.h>  // Unix standard header, using close

#include <cassert>  // using assert
#include <cerrno>  // using errno
#include <cstddef>  // using size_t
#include <cstdlib>  // exit
#include <cstring>  // using strncpy, strerror

#include <string.h>
#include <iostream>
#include <vector>
#include "./UnixDomainSocket.h"

class DomainSocketClient : public UnixDomainSocket {
 public:
  using UnixDomainSocket::UnixDomainSocket;

  void Run(int argc, char *argv[]);
  void MakeSocket();
  std::string ProcessString(int argc, char *argv[]);
  void ReceiveData(int);
};
#endif // DOMAINSOCKETCLIENT_H