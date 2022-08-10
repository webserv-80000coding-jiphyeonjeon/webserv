#include "Server.hpp"

#include <fcntl.h>
#include <unistd.h>

#include <cstdlib>
#include <iostream>

#include "Log.hpp"
#include "color.hpp"

Server::Server(const Server::ListenType& listen)
    : listen_(listen), socket_(-1), processor_map_() {
  setAddr();
}

Server::~Server() {}

const Server::FdType& Server::getFd() const { return socket_; }

const Server::ListenType& Server::getListen() const { return listen_; }

const ConfigServer::PortType& Server::getPort() const { return listen_.second; }

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

  fcntl(client_socket, F_SETFL, O_NONBLOCK);

  ft::log.writeTimeLog("[Server] --- Accept client ---");
  ft::log.getLogStream() << "Host: " << socket_ << "\nClient: " << client_socket
                         << std::endl;
  processor_map_.insert(std::make_pair(client_socket, Processor()));

  return client_socket;
}

RecvState Server::receiveData(Server::FdType client_socket) {
  Processor&                   processor = processor_map_.at(client_socket);
  const Processor::OffsetType& call_count = processor.getIoCount();
  processor.setIoCount(call_count + 1);

  char buffer[kBufferSize];
  memset(buffer, 0, kBufferSize);
  ssize_t recv_size = recv(client_socket, buffer, kBufferSize, 0);

  if (recv_size == 0 || recv_size == -1) {
    closeClient(client_socket);
    std::cout << (!recv_size ? BRED "\r🔌 Close  " END
                             : "\rRead error -> Close client")
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
    return kParseError;

  if (it->second.getLevel() == kDone) {
    std::cout << "\r" BCYN "📩 Receive" END << std::endl;
    ft::log.writeTimeLog("[Server] --- Receive Complete ---");
    ft::log.getLogStream() << "Client: " << client_socket
                           << "\nCount: " << processor.getIoCount()
                           << std::endl;

    ft::log.writeLog("Body:");
    std::string str = it->second.getRequest().getRequestMessage();
    (str.size() < 100) ? ft::log.writeLog(str)
                       : ft::log.writeLog(str.substr(0, 100) + "...");
    return kRecvSuccess;
  }
  return kRecvContinuous;
}

SendState Server::sendData(Server::FdType client_socket) {
  Processor&  processor = processor_map_.at(client_socket);
  std::string str = processor.getResponseMessage();

  const Processor::OffsetType& offset = processor.getOffset();
  const Processor::OffsetType& call_count = processor.getIoCount();
  processor.setIoCount(call_count + 1);

  ssize_t send_size =
      send(client_socket, str.c_str() + offset, str.size() - offset, 0);

  if (send_size == -1) {
    closeClient(client_socket);
    std::cout << "Send error" << std::endl;
    return kSendError;
  } else if (!offset && str.substr(9, 3) == "400") {
    closeClient(client_socket);
    std::cout << "\rConnection " RED "closed" END << std::endl;
    return kSendError;
  } else {
    processor.setOffset(offset + send_size);
    if (offset == str.size()) {
      std::cout << "\r" BGRN "📤 Send " END << std::endl;
      ft::log.writeTimeLog("[Server] --- Send Complete ---");
      ft::log.getLogStream() << "Client: " << client_socket
                             << "\nCount: " << call_count + 1 << std::endl;
      (str.size() < 100) ? ft::log.writeLog(str)
                         : ft::log.writeLog(str.substr(0, 100) + "...");

      processor_map_.erase(client_socket);
      processor_map_.insert(std::make_pair(client_socket, Processor()));
      return kSendSuccess;
    }
    return kSendContinuous;
  }
}

void Server::closeClient(Server::FdType client_socket) {
  if (client_socket > 0)
    close(client_socket);
  processor_map_.erase(client_socket);
}

void Server::process(Server::FdType client_socket, const Config& total_config) {
  processor_map_[client_socket].process(total_config, listen_);
}

void Server::setAddr() {
  memset(&addr_, 0, sizeof(addr_));
  addr_.sin_family = AF_INET;
  addr_.sin_addr.s_addr = listen_.first;
  addr_.sin_port = htons(listen_.second);
}

std::vector<Server::FdType> Server::checkExpiredConnection() {
  std::vector<Server::FdType> expired_clients;

  ProcessorType::iterator it = processor_map_.begin();
  for (; it != processor_map_.end(); it++) {
    if (it->second.isRequestExpired()) {
      expired_clients.push_back(it->first);
      ft::log.writeTimeLog("[Server] --- Close client ---");
      ft::log.getLogStream() << "Client: " << it->first << "\nReason: "
                             << "Request expired" << std::endl;
    }
  }

  for (std::vector<Server::FdType>::iterator it = expired_clients.begin();
       it != expired_clients.end(); it++)
    closeClient(*it);

  return expired_clients;
}
