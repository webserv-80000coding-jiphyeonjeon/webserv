#include "Server.hpp"

#include <fcntl.h>
#include <unistd.h>

#include <cstdlib>
#include <iostream>

Server::Server(const Server::ListenType& listen)
    : listen_(listen), socket_(-1), request_map_() {
  setAddr();
}

Server::~Server() {}

Server::FdType Server::getFd() const { return socket_; }

ConfigServer::PortType Server::getPort() const { return listen_.second; }

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

Server::FdType Server::acceptClient() {
  FdType             client_socket;
  struct sockaddr_in client_addr;
  socklen_t          client_addr_len = sizeof(client_addr);

  client_socket =
      accept(socket_, (struct sockaddr*)&client_addr, &client_addr_len);
  if (client_socket == -1)
    throw std::runtime_error("Server: accept() error");

  // 여기에 request를 위한 세팅 추가
  request_map_.insert(std::make_pair(client_socket, ""));

  return client_socket;
}

RecvState Server::recieveData(Server::FdType client_socket) {
  char buffer[kBufferSize];
  memset(buffer, 0, kBufferSize);
  ssize_t recv_size = recv(client_socket, buffer, kBufferSize, 0);

  if (recv_size == 0 || recv_size == -1) {
    closeClient(client_socket);
    std::cout << (!recv_size ? "\rConnection closed by client" : "\rRead error")
              << std::endl;
    return (!recv_size ? kClosedClient : kRecvError);
  }

  request_map_[client_socket] = std::string(buffer);
  return kRecvSuccess;
}

SendState Server::sendData(Server::FdType client_socket) {
  std::string str = request_map_[client_socket];
  ssize_t     send_size = send(client_socket, str.c_str(), str.size(), 0);

  if (send_size == -1) {
    closeClient(client_socket);
    std::cout << "Send error" << std::endl;
    return kSendError;
  } else {
    request_map_.erase(client_socket);
    return kSendSuccess;
  }
}

void Server::closeClient(Server::FdType client_socket) {
  if (client_socket > 0)
    close(client_socket);
  // request에 추가된 내용이 있었다면 삭제도 필요
  request_map_.erase(client_socket);
}

void Server::setAddr() {
  memset(&addr_, 0, sizeof(addr_));
  addr_.sin_family = AF_INET;
  addr_.sin_addr.s_addr = listen_.first;
  addr_.sin_port = htons(listen_.second);
}
