// Copyright (c) 2022 hyojekim. All rights reserved.

#ifndef WEBSERV_HPP_
#define WEBSERV_HPP_

#include "Server.hpp"

class Webserv {
 public:
  typedef Server::FdType           FdType;
  typedef std::map<FdType, Server> FdServerMapType;

  void initWebserv(const Config& config);
  void runWebserv();

 private:
  FdType          max_fd_;
  fd_set          fd_set_;
  FdServerMapType server_map_;
};

#endif  // WEBSERV_HPP_
