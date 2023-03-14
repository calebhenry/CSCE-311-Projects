/**
 * @file DomainSocketServer.cc
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-02-24
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "../inc/DomainSocketServer.h"

void DomainSocketServer:: RunServer() const {
  int sock_fd;
  int client_req_sock_fd;

  //  Create a socket
  sock_fd = CreateSocket();

  //  Listen for connections from clients
  size_t kMax_client_conns = get_nprocs_conf()-1;
  int success = listen(sock_fd, kMax_client_conns);
  if (success < 0) {
    std::cerr << strerror(errno) << std::endl;
    exit(-1);
  }

  std::clog << "SERVER STARTED\n\tMAX CLIENTS: " << kMax_client_conns
            << std::endl;

  const size_t kRead_buffer_size = 32;
  char read_buffer[kRead_buffer_size];
  int cycle_bytes_read;
  while (true) {
    //  Accept connection from a client
    client_req_sock_fd = accept(sock_fd, nullptr, nullptr);
    if (client_req_sock_fd  < 0) {
      std::cerr << strerror(errno) << std::endl;
      continue;
    }
    std::clog << "CLIENT CONNECTED" << std::endl;

    //  Receive data from client
    cycle_bytes_read = read(client_req_sock_fd, read_buffer, kRead_buffer_size);
    int total_bytes_read = 0;
    std::string message;
    const char kEoT = static_cast<char>(3);
    bool confirm = true;

    while (confirm) {
      total_bytes_read += cycle_bytes_read;
      message += read_buffer;
      message = message.substr(0, total_bytes_read);

      if (message.find(kEoT) != std::string::npos) {
        confirm = false;
        break;
      }
      cycle_bytes_read = read(client_req_sock_fd,
                              read_buffer, kRead_buffer_size);
    }

    if (cycle_bytes_read != 0) {
      std::vector<std::string> ret = RunSearch(message);
      SendData(ret, client_req_sock_fd);
      close(client_req_sock_fd);
    } else if (cycle_bytes_read < 0) {
      std::cerr << strerror(errno) << std::endl;

      exit(-1);
    }
  }
}

/**
 * @brief Creates a socket and binds it to an address
 * 
 * @return int The new socket fd
 */
int DomainSocketServer::CreateSocket() const {
  // Create a socket
  int sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);
  if ( sock_fd < 0 ) {
    std::cerr << strerror(errno) << std::endl;
    exit(-1);
  }
  // Bind socket to address for the server
  unlink(socket_path_.c_str());
  int success = bind(sock_fd,
                      reinterpret_cast<const sockaddr*>(&sock_addr_),
                      sizeof(sock_addr_));

  if (success < 0) {
    std::cerr << strerror(errno) << std::endl;
    exit(-1);
  }
  return sock_fd;
}

/**
 * @brief Takes a string, breaks it down, and searches the specified file for search
 * terms
 * 
 * @param str The string that will be processed
 * @return std::vector<std::string> The lines that contained the search terms
 */
std::vector<std::string> DomainSocketServer::RunSearch(std::string str) const {
  std::vector<std::string> expanded_str = Explode(str);
  std::vector<std::string> searched;
  std::ifstream file_read;
  std::string line;

  std::clog << "PATH: " << expanded_str[0] << std::endl;
  if (expanded_str[1] == "+") {
    std::clog << "OPERATION: OR" << std::endl;
  } else if (expanded_str[1] == "x") {
    std::clog << "OPERATION: AND" << std::endl;
  } else {
    std::clog << "OPERATION: n/a" << std::endl;
  }
  std::clog << "SEEKING: ";
  for (size_t i = 2; i < expanded_str.size(); i++) {
    if (i == expanded_str.size()-1) {
      std::clog << expanded_str[i] << std::endl;
    } else {
      std::clog << expanded_str[i] << ", ";
    }
  }

  file_read.open(expanded_str[0]);
  if (file_read.is_open()) {
    while (std::getline(file_read, line)) {
      if (expanded_str[1] == "x") {
        bool add = true;
        for (size_t i = 2; i < expanded_str.size(); i++) {
          if (line.find(expanded_str[i]) == std::string::npos) {
            add = false;
          }
        }
        if (add) {
          searched.push_back(line);
        }
      } else if (expanded_str[1] == "+") {
        bool add = false;
        for (size_t i = 2; i < expanded_str.size(); i++) {
          if (line.find(expanded_str[i]) != std::string::npos) {
            add = true;
          }
        }
        if (add) {
          searched.push_back(line);
        }
      } else {
        if (line.find(expanded_str[2]) != std::string::npos) {
          searched.push_back(line);
        }
      }
    }
    file_read.close();
  } else {
    searched.resize(1);
    searched[0] = static_cast<char>(3);
  }
  return searched;
}

/**
 * @brief Explodes a delimited string into a vector
 * 
 * @param str The string that is to be exploded
 * @return std::vector<std::string> The exploded string
 */
std::vector<std::string> DomainSocketServer::Explode(std::string str) const {
  const char kUS = static_cast<char>(31);
  const char kEoT = static_cast<char>(3);
  std::vector<std::string> expanded_str;
  while (str.find(kUS) != std::string::npos) {
    expanded_str.push_back(str.substr(0, str.find(kUS)));
    str = str.substr(str.find(kUS)+1);
    if (str.find(kUS) == std::string::npos &&
        str.find(kEoT) != std::string::npos) {
      expanded_str.push_back(str.substr(0, str.find(kEoT)));
    }
  }
  return expanded_str;
}

/**
 * @brief Packages the lines into a string and sends them to the client
 * 
 * @param lines The lines that will be sent
 * @param sock_fd The socket file descriptor
 */
void DomainSocketServer::SendData(std::vector<std::string> lines,
                                  int sock_fd) const {
  const char kUS = static_cast<char>(31);
  const char kEoT = static_cast<char>(3);
  std::string message;
  for (auto str : lines) {
    message += str + kUS;
  }
  message += kEoT;
  const ssize_t kWrite_buffer_size = 64;
  std::string buff;
  std::string remain;

  if (message.length() > 64) {
    buff = message.substr(0, kWrite_buffer_size);
    remain = message.substr(kWrite_buffer_size);
  } else {
    buff = message;
  }

  bool cont = true;
  int total_bytes_wrote = 0;

  while (cont) {
    ssize_t bytes_wrote = write(sock_fd, buff.data(), buff.size());
    total_bytes_wrote += buff.size();
    if (bytes_wrote < 0) {
      std::cerr << strerror(errno) << std::endl;
      exit(-1);
    }

    if (bytes_wrote == 0) {
      std::clog << "Client dropped connection!" << std::endl;
      exit(-2);
    }

    if (remain.length() > 64) {
      buff = remain.substr(0, kWrite_buffer_size);
      remain = remain.substr(kWrite_buffer_size);
    } else if (remain.length() != 0) {
      buff = remain;
      remain = "";
    } else {
      cont = false;
    }
  }
  std::clog << "BYTES SENT: " << total_bytes_wrote << std::endl;
}
