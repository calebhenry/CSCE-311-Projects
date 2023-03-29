/**
 * @file UnixDomainSocket.cc
 * @author Caleb Henry
 * @brief  Creates the struct for the domain socket
 * @version 0.1
 * @date 2023-03-13
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "../inc/UnixDomainSocket.h"

/**
 * @brief Creates a unix domain socket struct
 * 
 * @param socket_path Path name of the socket
 * @param abstract If the socket is abstract or not
 * @return UnixDomainSocket The instance of the socket
 */
UnixDomainSocket ::UnixDomainSocket(const char *socket_path, bool abstract) {
  socket_path_ = std::string(socket_path);  // std::string manages char *

  sock_addr_ = {};  // init struct (replaces memset from C)
  sock_addr_.sun_family = AF_UNIX;  // set to Unix domain socket (e.g. instead
                                    //   of internet domain socket)
  if (abstract)
    // leaving leading null char sets abstract socket
    strncpy(sock_addr_.sun_path + 1,           // strncpy to limit copy for
            socket_path,                       //   portability
            sizeof(sock_addr_.sun_path) - 1);  // -2 for leading/trailing \0s
  else
    // copy string from socket path without leading \0
    strncpy(sock_addr_.sun_path,               // strncpy to limit copy for
            socket_path,                       //   portability
            sizeof(sock_addr_.sun_path));  //
}
