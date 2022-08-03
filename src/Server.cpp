#include "Server.hpp"

#include <fcntl.h>
#include <unistd.h>

#include <cstdlib>
#include <iostream>

#include "Log.hpp"

Server::Server(const Server::ListenType& listen)
    : listen_(listen), socket_(-1), processor_map_() {
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

  // 연결된 fd와 여기에 관련된 Processor 객체를 map에 추가
  processor_map_.insert(std::make_pair(client_socket, Processor()));
  ft::log.writeTimeLog("[Server] --- Accept client ---");
  ft::log.getLogStream() << "Host: " << socket_ << "\nClient: " << client_socket
                         << std::endl;

  return client_socket;
}

RecvState Server::receiveData(Server::FdType client_socket) {
  ft::log.writeTimeLog("[Server] --- Receive data ---");

  char buffer[kBufferSize];
  memset(buffer, 0, kBufferSize);
  ssize_t recv_size = recv(client_socket, buffer, kBufferSize, 0);

  if (recv_size == 0 || recv_size == -1) {
    closeClient(client_socket);
    std::cout << (!recv_size ? "\rConnection closed by client" : "\rRead error")
              << std::endl;
    ft::log.writeTimeLog("[Server] --- Close client ---");
    ft::log.getLogStream() << "Client: " << client_socket << "\nReason: "
                           << (!recv_size ? "Connection closed by client"
                                          : "Read error")
                           << std::endl;
    return (!recv_size ? kClosedClient : kRecvError);
  }

  ProcessorType::iterator it = processor_map_.find(client_socket);
  if (it->second.parseRequest(std::string(buffer)) == -1)
    return kRecvError;
  return (it->second.getLevel() == kDone ? kRecvSuccess : kRecvContinuous);
}

SendState Server::sendData(Server::FdType client_socket) {
  std::string str = processor_map_[client_socket].strRequest();
  ft::log.writeTimeLog("[Server] --- Send Data ---");
  ft::log.writeLog(str);
  ssize_t send_size = send(client_socket, str.c_str(), str.size(), 0);

  if (send_size == -1) {
    closeClient(client_socket);
    std::cout << "Send error" << std::endl;
    return kSendError;
  } else {
    processor_map_.erase(client_socket);
    return kSendSuccess;
  }
}

void Server::closeClient(Server::FdType client_socket) {
  if (client_socket > 0)
    close(client_socket);
  processor_map_.erase(client_socket);
}

void Server::setAddr() {
  memset(&addr_, 0, sizeof(addr_));
  addr_.sin_family = AF_INET;
  addr_.sin_addr.s_addr = listen_.first;
  addr_.sin_port = htons(listen_.second);
}
