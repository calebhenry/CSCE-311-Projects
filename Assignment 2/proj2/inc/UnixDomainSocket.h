/**
 * @file UnixDomainSocket.h
 * @author Caleb Henry
 * @brief 
 * @version 0.1
 * @date 2023-02-24
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef UNIXDOMAINSOCKET_H
#define UNIXDOMAINSOCKET_H

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

class  UnixDomainSocket {
 public:
  ///
  /// Initializes the socket address struct (::sockaddr_un)
  ///
  /// @param socket_path A null-terminated C-string containing the shared path
  ///                    name for socket.
  /// @param abstract Defaults to abstract socket path names, pass false for
  ///                 non-abstract naming.
  /// 
  explicit UnixDomainSocket(const char *socket_path, bool abstract = true);

 protected:
  ::sockaddr_un sock_addr_;  // socket address from sys/un.h

  std::string socket_path_;  // std::string stores socket_path (no raw pointers)
};

#endif // UNIXDOMAINSOCKET_H