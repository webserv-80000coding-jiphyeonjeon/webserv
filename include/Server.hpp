// Copyright (c) 2022 hyojekim. All rights reserved.

#ifndef SERVER_HPP_
#define SERVER_HPP_

#include <arpa/inet.h>
#include <sys/socket.h>

#include "Config.hpp"

class Server {
 public:
  typedef ConfigServer::ListenType ListenType;
  typedef int                      FdType;

  Server(const ListenType& listen);
  ~Server();

  void   initServer();
  FdType getFd();

 private:
  static const int kBacklog = 1024;

  ListenType         listen_;
  int                socket_;
  struct sockaddr_in addr_;

  void setAddr();
};

#endif  // SERVER_HPP_
