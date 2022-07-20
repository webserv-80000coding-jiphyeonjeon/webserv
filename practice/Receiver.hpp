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
  int                server_socket_;
  struct sockaddr_in server_address_;

  static fd_set read_fds_;
  static int    fd_max_;

  void error_handling(const char* buf);

 public:
  Receiver(int port);
  void bind_and_listen();

  int&       get_server_socket();
  const int& get_server_socket() const;

  static fd_set& get_read_fds();
  static int&    get_fd_max();

  struct BindException : public std::exception {
    const char* what() const throw() { return "BindException"; }
  };

  struct ListenException : public std::exception {
    const char* what() const throw() { return "ListenException"; }
  };
};

#endif  // WEBSERV_PRACTICE_RECEIVER_HPP_