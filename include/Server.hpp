// Copyright (c) 2022 hyojekim. All rights reserved.

#ifndef SERVER_HPP_
#define SERVER_HPP_

#include <arpa/inet.h>
#include <sys/socket.h>

#include <vector>

#include "Config.hpp"
#include "Processor.hpp"

enum RecvState {
  kClosedClient = 0,
  kRecvSuccess = 1,
  kRecvContinuous = 2,
  kRecvError = -1,
  kParseError = -2
};

enum SendState { kSendError = -1, kSendSuccess = 0, kSendContinuous = 1 };

class Server {
 public:
  typedef ConfigServer::ListenType    ListenType;
  typedef int                         FdType;
  typedef std::map<FdType, Processor> ProcessorType;

  Server(const ListenType& listen);
  ~Server();

  const FdType&                 getFd() const;
  const ListenType&             getListen() const;
  const ConfigServer::PortType& getPort() const;

  void      initServer();
  FdType    acceptClient();
  RecvState receiveData(FdType client_socket);
  SendState sendData(FdType client_socket);
  void      closeClient(FdType client_socket);

  void process(FdType client_socket, const Config& total_config);

  std::vector<FdType> checkExpiredConnection();

 private:
  static const int kBacklog = 1024;
  static const int kBufferSize = 1024;

  ListenType         listen_;
  FdType             socket_;
  struct sockaddr_in addr_;
  ProcessorType      processor_map_;

  void setAddr();
};

#endif  // SERVER_HPP_
