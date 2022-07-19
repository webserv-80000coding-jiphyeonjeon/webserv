#include "Receiver.hpp"

#include <fcntl.h>

#include <iostream>

Receiver::Receiver(int port) : port_(port) {}

void Receiver::error_handling(const char* buf) {
  std::cerr << buf << std::endl;
  exit(1);
}

void Receiver::run() {
  socklen_t addr_size;
  int       fd_max, str_len, fd_num, i;
  char      buf[BUF_SIZE];
  bool      is_on = true;

  // 이렇게 private 멤버 변수 초기화는 생성자에서 하는 게 좋을까나...?
  server_socket_ = socket(PF_INET, SOCK_STREAM, 0);
  memset(&server_address_, 0, sizeof(server_address_));
  server_address_.sin_family = AF_INET;
  server_address_.sin_addr.s_addr = htonl(INADDR_ANY);
  server_address_.sin_port = htons(port_);

  fcntl(server_socket_, F_SETFL, O_NONBLOCK);
  setsockopt(server_socket_, SOL_SOCKET, SO_REUSEADDR, &is_on, sizeof(int));

  std::cout << port_ << std::endl;
  if (bind(server_socket_, (struct sockaddr*)&server_address_, sizeof(server_address_)) == -1)
    error_handling("bind() error");
  if (listen(server_socket_, 5) == -1)
    error_handling("listen() error");

  FD_ZERO(&read_fds_);                 // set reads to the null set
  FD_SET(server_socket_, &read_fds_);  // include serv_sock in reads
  fd_max = server_socket_;             // check max fd to use in select()

  while (1) {
    cp_read_fds_ = read_fds_;

    timeout_.tv_sec = 5;
    timeout_.tv_usec = 5000;

    if ((fd_num = select(fd_max + 1, &cp_read_fds_, 0, 0, &timeout_)) == -1)
      break;
    if (fd_num == 0)  // means time limit is expired
      continue;
    for (i = 0; i < fd_max + 1; i++) {
      if (FD_ISSET(i, &cp_read_fds_)) {
        if (i == server_socket_) {
          addr_size = sizeof(client_address_);
          client_socket_ = accept(server_socket_, (struct sockaddr*)&client_address_, &addr_size);
          FD_SET(client_socket_, &read_fds_);
          if (fd_max < client_socket_)
            fd_max = client_socket_;
          std::cout << "connected client: " << i << std::endl;
        } else {
          if ((str_len = read(i, buf, BUF_SIZE)) == 0) {
            FD_CLR(i, &read_fds_);
            close(i);
            std::cout << "closed client: " << i << std::endl;
            break;
          } else {
            write(1, buf, str_len);
            write(i, buf, str_len);
          }
        }
      }
    }
  }
  close(server_socket_);
  return;
}