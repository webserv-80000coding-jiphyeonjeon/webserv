// Copyright (c) 2022 hyojekim. All rights reserved.

#ifndef SERVER_HPP_
#define SERVER_HPP_

#include <arpa/inet.h>
#include <sys/socket.h>

#include "Config.hpp"

enum RecvState { kClosedClient = 0, kRecvError = -1, kRecvSuccess = 1 };
enum SendState { kSendError = -1, kSendSuccess = 0 };

class Server {
 public:
  typedef ConfigServer::ListenType      ListenType;
  typedef int                           FdType;
  typedef std::map<FdType, std::string> RequestType;

  Server(const ListenType& listen);
  ~Server();

  FdType                 getFd() const;
  ConfigServer::PortType getPort() const;

  void      initServer();
  FdType    acceptClient();
  RecvState recieveData(FdType client_socket);
  SendState sendData(FdType client_socket);
  void      closeClient(FdType client_socket);

 private:
  static const int kBacklog = 1024;
  static const int kBufferSize = 1024;

  ListenType         listen_;
  int                socket_;
  struct sockaddr_in addr_;
  RequestType        request_map_;

  void setAddr();
};

#endif  // SERVER_HPP_
