#include "Receiver.hpp"

#include <fcntl.h>

#include <iostream>

fd_set Receiver::read_fds_;
int    Receiver::fd_max_ = 0;

Receiver::Receiver(int port) : port_(port) {}

int&       Receiver::get_server_socket() { return server_socket_; }
const int& Receiver::get_server_socket() const { return server_socket_; }

fd_set& Receiver::get_read_fds() { return read_fds_; }
int&    Receiver::get_fd_max() { return fd_max_; }

void Receiver::error_handling(const char* buf) {
  std::cerr << buf << std::endl;
  exit(1);
}

void Receiver::bind_and_listen() {
  bool is_on = true;

  server_socket_ = socket(PF_INET, SOCK_STREAM, 0);
  memset(&server_address_, 0, sizeof(server_address_));
  server_address_.sin_family = AF_INET;
  server_address_.sin_addr.s_addr = htonl(INADDR_ANY);
  server_address_.sin_port = htons(port_);

  // set socket to non-blocking mode
  // F_SETFL: set file status flags
  fcntl(server_socket_, F_SETFL, O_NONBLOCK);

  // set socket to reusable address
  setsockopt(server_socket_, SOL_SOCKET, SO_REUSEADDR, &is_on, sizeof(int));

  if (bind(server_socket_, (struct sockaddr*)&server_address_, sizeof(server_address_)) == -1) {
    error_handling("bind() error");
  }
  if (listen(server_socket_, 5) == -1) {
    error_handling("listen() error");
  }

  FD_SET(server_socket_, &read_fds_);  // include serv_sock in reads

  fd_max_ = fd_max_ > server_socket_ ? fd_max_ : server_socket_;
}
