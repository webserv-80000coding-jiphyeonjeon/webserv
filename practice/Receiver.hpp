#ifndef WEBSERV_PRACTICE_RECEIVER_HPP_
#define WEBSERV_PRACTICE_RECEIVER_HPP_

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

#include <exception>

#define BUF_SIZE 1024

class Receiver {
 private:
  int                port_;
  int                server_socket_, client_socket_;
  struct sockaddr_in server_address_, client_address_;
  struct timeval     timeout_;
  fd_set             read_fds_, cp_read_fds_;

  void error_handling(const char *buf);

 public:
  // Receiver();
  Receiver(int port);
  // ~Receiver();
  void run();

  struct BindException : public std::exception {
    const char *what() const throw() { return "BindException"; }
  };

  struct ListenException : public std::exception {
    const char *what() const throw() { return "ListenException"; }
  };
};

#endif  // WEBSERV_PRACTICE_RECEIVER_HPP_