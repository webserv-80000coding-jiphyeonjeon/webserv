#include "Config.hpp"

#include <iostream>

#include "color.hpp"

Config::Config() : servers_() {}

Config::~Config() {}

const Config::ServersType& Config::getServers() const { return servers_; }

void Config::addServers(const ConfigServer& server) { servers_.push_back(server); }

void Config::printConfig() const {
  for (ServersType::const_iterator server = servers_.begin(); server != servers_.end(); ++server) {
    std::cout << BWHT "📮 server block" END << std::endl;
    printServer(*server);
    std::cout << std::endl;
  }
}

void Config::printServer(const ConfigServer& server) const {
  const ServerNameType server_name = server.getServerName();
  for (ServerNameType::const_iterator it = server_name.begin(); it != server_name.end(); ++it) {
    std::cout << GRN "[ server name ]" END << std::endl;
    std::cout << *it << std::endl;
  }
}
