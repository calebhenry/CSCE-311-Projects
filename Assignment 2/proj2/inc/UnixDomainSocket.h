/**
 * @file UnixDomainSocket.h
 * @author Caleb Henry
 * @brief Creates the struct for the domain socket
 * @version 0.1
 * @date 2023-02-24
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef INC_UNIXDOMAINSOCKET_H_
#define INC_UNIXDOMAINSOCKET_H_

#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include <cassert>
#include <cerrno>
#include <cstddef>
#include <cstdlib>
#include <cstring>

#include <string>
#include <iostream>

class  UnixDomainSocket {
 public:
  explicit UnixDomainSocket(const char *socket_path, bool abstract = true);

 protected:
  ::sockaddr_un sock_addr_;  // socket address from sys/un.h

  std::string socket_path_;  // std::string stores socket_path (no raw pointers)
};

#endif  // INC_UNIXDOMAINSOCKET_H_
