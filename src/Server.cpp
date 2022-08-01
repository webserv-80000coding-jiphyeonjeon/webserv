#include "Server.hpp"

#include <fcntl.h>

#include <cstdlib>

Server::Server(const Server::ListenType& listen)
    : listen_(listen), socket_(-1) {
  setAddr();
}

Server::~Server() {}

void Server::initServer() {
  if ((socket_ = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    throw std::runtime_error("Server: socket() error");

  fcntl(socket_, F_SETFL, O_NONBLOCK);
  bool is_on = true;
  setsockopt(socket_, SOL_SOCKET, SO_REUSEADDR, &is_on, sizeof(int));

  if (bind(socket_, (struct sockaddr*)&addr_, sizeof(addr_)) == -1)
    throw std::runtime_error("Server: bind() error");

  if (listen(socket_, kBacklog) == -1)
    throw std::runtime_error("Server: listen() error");
}

void Server::setAddr() {
  memset(&addr_, 0, sizeof(addr_));
  addr_.sin_family = AF_INET;
  addr_.sin_addr.s_addr = listen_.first;
  addr_.sin_port = htons(listen_.second);
}

Server::FdType Server::getFd() { return socket_; }
