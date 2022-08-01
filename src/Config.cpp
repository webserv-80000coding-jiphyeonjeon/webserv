#include "Config.hpp"

// TODO 디버깅용으로 사용한 inet_ntoa() 제거 필요!!!
#include <arpa/inet.h>

#include <iostream>

#include "color.hpp"

Config::Config() : servers_() {}

Config::~Config() {}

const Config::ServersType& Config::getServers() const { return servers_; }

void Config::addServers(const ConfigServer& server) {
  servers_.push_back(server);
}

Config::ListenListType Config::getAllListenList() const {
  ListenListType                     all_listen_list;
  std::set<ConfigServer::ListenType> unique_list;

  for (ServersType::const_iterator server = servers_.begin();
       server != servers_.end(); ++server) {
    const ListenListType& listen_list = server->getListen();
    for (ListenListType::const_iterator listen = listen_list.begin();
         listen != listen_list.end(); ++listen) {
      if ((unique_list.insert(*listen)).second)
        all_listen_list.push_back(*listen);
    }
  }
  return all_listen_list;
}

void Config::printConfig() const {
  int i = 0;
  for (ServersType::const_iterator server = servers_.begin();
       server != servers_.end(); ++server, ++i) {
    std::cout << BGRN "📮 server [" << i << "]" END << std::endl;
    printServer(*server);
    std::cout << std::endl;
  }
}

void Config::printServer(const ConfigServer& server) const {
  std::cout << GRN "  [server name]" END << std::endl;
  const ServerNameType server_name = server.getServerName();
  for (ServerNameType::const_iterator it = server_name.begin();
       it != server_name.end(); ++it)
    std::cout << "    " << *it << "\n";
  std::cout << std::endl;

  std::cout << GRN "  [listen]" END << std::endl;
  printListen(server.getListen(), "    ");

  printCommon(server.getCommon(), "  ", GRN);
  std::cout << std::endl;

  const LocationType locations = server.getLocation();
  for (LocationType::const_iterator loc = locations.begin();
       loc != locations.end(); ++loc) {
    std::cout << BYEL "  📂 location " << loc->first << END << std::endl;
    printLocation(loc->second, "    ");
    std::cout << std::endl;
  }
}

void Config::printLocation(const ConfigLocation& location,
                           const std::string&    indent) const {
  std::cout << indent << YEL "  [limit_except]" END << std::endl;
  const LimitExceptType limit_except = location.getLimitExcept();
  for (LimitExceptType::const_iterator it = limit_except.begin();
       it != limit_except.end(); ++it)
    std::cout << indent << "    " << *it << "\n";
  std::cout << std::endl;

  printCommon(location.getCommon(), indent + "  ", YEL);
}

void Config::printCommon(const ConfigCommon& common, const std::string& indent,
                         const std::string& color) const {
  std::cout << indent << color << "[autoindex] " END
            << (common.getAutoindex() ? "on" : "off") << "\n"
            << std::endl;
  std::cout << indent << color << "[client_body_buffer_size] " END
            << common.getClientBodyBufferSize() << "\n"
            << std::endl;
  std::cout << indent << color << "[root] " END << common.getRoot() << "\n"
            << std::endl;

  std::cout << indent << color << "[index]" END << std::endl;
  const IndexType index = common.getIndex();
  for (IndexType::const_iterator it = index.begin(); it != index.end(); ++it)
    std::cout << indent << "  " << *it << "\n";
  std::cout << std::endl;

  std::cout << indent << color << "[error_page]" END << std::endl;
  const ErrorPageType error_page = common.getErrorPage();
  for (ErrorPageType::const_iterator it = error_page.begin();
       it != error_page.end(); ++it)
    std::cout << indent << "  " << it->first << " -> " << it->second << "\n";
  std::cout << std::endl;
}

void Config::printListen(const Config::ListenListType& listen_list,
                         const std::string&            indent) const {
  for (ListenListType::const_iterator it = listen_list.begin();
       it != listen_list.end(); ++it) {
    sockaddr_in addr;
    addr.sin_addr.s_addr = it->first;
    std::cout << indent << inet_ntoa(addr.sin_addr) << ":" << it->second
              << "\n";
  }
  std::cout << std::endl;
}
