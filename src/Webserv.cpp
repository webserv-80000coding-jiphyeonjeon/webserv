#include "Webserv.hpp"

#include <iostream>

#include "color.hpp"

void Webserv::initWebserv(const Config& config) {
  Config::ListenListType listen_list = config.getAllListenList();
  config.printListen(listen_list, "");

  FD_ZERO(&fd_set_);
  max_fd_ = 0;

  for (Config::ListenListType::const_iterator listen = listen_list.begin();
       listen != listen_list.end(); ++listen) {
    Server server(*listen);
    FdType fd;

    server.initServer();
    fd = server.getFd();
    FD_SET(fd, &fd_set_);
    server_map_.insert(std::make_pair(fd, server));
    if (fd > max_fd_)
      max_fd_ = fd;

    sockaddr_in addr;
    addr.sin_addr.s_addr = listen->first;
    std::cout << BWHT "📮  " << inet_ntoa(addr.sin_addr) << ":" << listen->second
              << END " listen..." << std::endl;

    if (!max_fd_)
      throw std::runtime_error("Server: No listening.");
  }
}

void Webserv::runWebserv() {
  while (1) {
  }
}
