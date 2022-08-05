// Copyright (c) 2022 hyojekim. All rights reserved.

#ifndef WEBSERV_HPP_
#define WEBSERV_HPP_

#include "Server.hpp"

class Webserv {
 public:
  typedef Server::FdType            FdType;
  typedef std::map<FdType, Server>  FdServerMapType;
  typedef std::map<FdType, Server*> ConnectSocketType;
  typedef std::vector<FdType>       ReadyType;

  Webserv(const Config& config);
  ~Webserv();

  void initWebserv();
  void runWebserv();

 private:
  static const int kTimeOut = 0;
  static const int kError = -1;

  const Config&     config_;
  FdType            max_fd_;
  fd_set            fd_set_;
  FdServerMapType   server_map_;
  ConnectSocketType connect_socket_;
  ReadyType         ready_to_write_;

  void addConnection(int& state, fd_set& read_fds);
  void receiveRequest(int& state, fd_set& read_fds);
  void sendResponse(int& state, fd_set& write_fds);
  void selectError();
};

#endif  // WEBSERV_HPP_
