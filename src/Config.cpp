#include "Config.hpp"

// TODO 디버깅용으로 사용한 inet_ntoa() 제거 필요!!!
#include <arpa/inet.h>

#include <iostream>

#include "color.hpp"

Config::Config() : servers_() {}

Config::~Config() {}

const Config::ServersType& Config::getServers() const { return servers_; }

void Config::addServers(const ConfigServer& server) { servers_.push_back(server); }

void Config::printConfig() const {
  int i = 0;
  for (ServersType::const_iterator server = servers_.begin(); server != servers_.end();
       ++server, ++i) {
    std::cout << BWHT "📮 server [" << i << "]" END << std::endl;
    printServer(*server);
    std::cout << std::endl;
  }
}

void Config::printServer(const ConfigServer& server) const {
  std::cout << GRN "  [server name]" END << std::endl;
  const ServerNameType server_name = server.getServerName();
  for (ServerNameType::const_iterator it = server_name.begin(); it != server_name.end(); ++it) {
    std::cout << "    " << *it << std::endl;
  }
  std::cout << std::endl;

  std::cout << GRN "  [listen]" END << std::endl;
  const ListenListType listen = server.getListen();
  for (ListenListType::const_iterator it = listen.begin(); it != listen.end(); ++it) {
    sockaddr_in addr;
    addr.sin_addr.s_addr = it->first;
    std::cout << "    " << inet_ntoa(addr.sin_addr) << ":" << it->second << std::endl;
  }
  std::cout << std::endl;
}
