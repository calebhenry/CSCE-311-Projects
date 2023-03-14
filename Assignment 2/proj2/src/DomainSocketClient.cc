/**
 * @file DomainSocketClient.cc
 * @author Caleb Henry
 * @brief Holds the Run function (along with its helper functions) 
 * which connects and shares information with its server counterpart
 * @version 0.1
 * @date 2023-02-24
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "../inc/DomainSocketClient.h"

/**
 * @brief Takes in command line arguments and sends them to the server.
 * The first argument is the socket name, the second is the filepath to
 * look in, and all of the arguments after that are what is to be searched
 * for.
 * 
 * @param argc The argument count
 * @param argv  The argument vector
 */
void DomainSocketClient ::RunClient(int argc, char *argv[]) {
  // Process and delimit string
  std::string message = ProcessString(argc, argv);

  // Open a socket
  int socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
  if (socket_fd < 0) {
    std::cerr << strerror(errno) << std::endl;
    exit(-1);
  }

  //  Connect to the shared socket
  int success = connect(socket_fd,
                        reinterpret_cast<const sockaddr*>(&sock_addr_),
                        sizeof(sock_addr_));
  if (success < 0) {
    std::cerr << strerror(errno) << std::endl;
    exit(-1);
  }
  std::cout << "SERVER CONNECTION ACCEPTED" << std::endl;

  //  Write to socket
  const ssize_t kWrite_buffer_size = 64;
  std::string buff;
  std::string remain;

  if (message.length() > 64) {
    buff = message.substr(0, kWrite_buffer_size-1);
    remain = message.substr(kWrite_buffer_size);
  } else {
    buff = message;
  }
  bool cont = true;
  while (cont) {
    ssize_t bytes_wrote = write(socket_fd, buff.data(), buff.size());
    if (bytes_wrote < 0) {
      std::cerr << strerror(errno) << std::endl;
      exit(-1);
    }

    if (bytes_wrote == 0) {
      std::clog << "Server dropped connection!" << std::endl;
      exit(-2);
    }

    if (remain.length() > 64) {
      buff = remain.substr(0, kWrite_buffer_size-1);
      remain = message.substr(kWrite_buffer_size);
    } else if (remain.length() != 0) {
      buff = remain;
      remain = "";
    } else {
      cont = false;
    }
  }
  ReceiveData(socket_fd);
}

/**
 * @brief Takes the command line arguments and processes it into a single
 * delimited and ready to send string
 * 
 * @param argc The argument count
 * @param argv The argument vector
 * @return std::string The delimited string
 */
std::string DomainSocketClient::ProcessString(int argc, char *argv[]) {
  char kUS = static_cast<char>(31);
  char kEoT = static_cast<char>(3);
  std::string final_str;  //  Contains file location, followed by
                          //  unit seperator, followed by opcode,
                          //  followed by unit seperator, followed
                          //  by search strings, followed by end
                          //  of transmission
  std::string search = "";
  std::string op_code = "";

  final_str = argv[2];
  final_str.append(1, kUS);
  if (argc == 4) {
    final_str += '-';
    final_str += kUS;
    final_str += argv[3];
    final_str += kEoT;
    return final_str;
  }
  op_code = argv[4];
  for (int i = 3; i < argc-1; i++) {
    if (i % 2 != 0) {
      search += argv[i];
      search += kUS;
    } else {
      if (op_code != argv[i]) {
        std::cerr << "Mixed boolean operators not presently supported"
          << std::endl;
        exit(2);
      }
    }
  }
  final_str += op_code + kUS + search + argv[argc-1];
  final_str += kEoT;
  return final_str;
}

/**
 * @brief Reads/processes any incoming data, then prints it
 * 
 * @param sock_fd The socket file descriptor that is being read from
 */
void DomainSocketClient::ReceiveData(int sock_fd) {
  const size_t kRead_buffer_size = 32;
  char read_buffer[kRead_buffer_size];
  int cycle_bytes_read;
  while (true) {
    cycle_bytes_read = read(sock_fd, read_buffer, kRead_buffer_size);
    int total_bytes_read = 0;
    std::string message;
    const char kUS = static_cast<char>(31);
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

      cycle_bytes_read = read(sock_fd, read_buffer, kRead_buffer_size);
    }

    if (cycle_bytes_read >= 0) {
      std::vector<std::string> expanded_str;
      while (message.find(kUS) != std::string::npos) {
        expanded_str.push_back(message.substr(0, message.find(kUS)));
        message = message.substr(message.find(kUS)+1);
      }

      if (expanded_str[0].find(kEoT) != std::string::npos) {
        std::cout << "INVALID FILE" << std::endl;
        exit(0);
      }

      int index = 1;
      for (auto str : expanded_str) {
        std::cout << index << "\t" << str << std::endl;
        index++;
      }
      std::cout << "BYTES RECEIVED: " << total_bytes_read << std::endl;
      exit(0);
    } else if (cycle_bytes_read < 0) {
      std::cerr << strerror(errno) << std::endl;

      exit(-1);
    }
  }
}
