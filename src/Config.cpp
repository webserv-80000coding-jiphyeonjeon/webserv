#include "Config.hpp"

// TODO 디버깅용으로 사용한 inet_ntoa() 제거 필요!!!
#include <arpa/inet.h>

#include <iostream>

#include "Log.hpp"
#include "color.hpp"

Config::Config() : servers_() {}

Config::~Config() {}

const Config::ServersType& Config::getServers() const { return servers_; }

void Config::addServers(const ConfigServer& server) {
  servers_.push_back(server);
}

Config::ListenListType Config::getAllListenList() const {
  ListenListType       all_listen_list;
  std::set<ListenType> unique_list;

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
  ft::log.writeTimeLog("[Config] --- Print config after parsing ---");

  int i = 0;
  for (ServersType::const_iterator server = servers_.begin();
       server != servers_.end(); ++server, ++i) {
    ft::log.getLogStream() << BGRN "📮 server [" << i << "]" END << std::endl;
    printServer(*server);
    ft::log.writeEndl();
  }
}

void Config::printServer(const ConfigServer& server) {
  ft::log.getLogStream() << GRN "  [server name]" END << std::endl;
  const ServerNamesType server_name = server.getServerName();
  for (ServerNamesType::const_iterator it = server_name.begin();
       it != server_name.end(); ++it)
    ft::log.getLogStream() << "    " << *it << "\n";
  ft::log.writeEndl();

  ft::log.writeLog(GRN "  [listen]" END);
  printListen(server.getListen(), "    ");

  printCommon(server.getCommon(), "  ", GRN);
  ft::log.writeEndl();

  const LocationType locations = server.getLocation();
  for (LocationType::const_iterator loc = locations.begin();
       loc != locations.end(); ++loc) {
    ft::log.getLogStream() << BYEL "  📂 location " << loc->first << END
                           << std::endl;
    printLocation(loc->second, "    ");
    ft::log.writeEndl();
  }
}

void Config::printLocation(const ConfigLocation& location,
                           const std::string&    indent) {
  ft::log.getLogStream() << indent << YEL "  [cgi]" END << std::endl;
  const CgiType cgi = location.getCgi();
  for (CgiType::const_iterator it = cgi.begin(); it != cgi.end(); ++it)
    ft::log.getLogStream() << indent << "    " << it->first << " " << it->second
                           << "\n";
  ft::log.writeEndl();

  ft::log.getLogStream() << indent << YEL "  [limit_except]" END << std::endl;
  const LimitExceptType limit_except = location.getLimitExcept();
  for (LimitExceptType::const_iterator it = limit_except.begin();
       it != limit_except.end(); ++it)
    ft::log.getLogStream() << indent << "    " << *it << "\n";
  ft::log.writeEndl();

  printCommon(location.getCommon(), indent + "  ", YEL);
}

void Config::printCommon(const ConfigCommon& common, const std::string& indent,
                         const std::string& color) {
  ft::log.getLogStream() << indent << color << "[autoindex] " END
                         << (common.getAutoindex() ? "on" : "off") << "\n"
                         << std::endl;
  ft::log.getLogStream() << indent << color << "[client_body_buffer_size] " END
                         << common.getClientBodyBufferSize() << "\n"
                         << std::endl;
  ft::log.getLogStream() << indent << color << "[root] " END << common.getRoot()
                         << "\n"
                         << std::endl;

  const ReturnType return_pair = common.getReturn();
  ft::log.getLogStream() << indent << color << "[return] " END
                         << return_pair.first << " " << return_pair.second
                         << "\n"
                         << std::endl;

  ft::log.getLogStream() << indent << color << "[index]" END << std::endl;
  const IndexType index = common.getIndex();
  for (IndexType::const_iterator it = index.begin(); it != index.end(); ++it)
    ft::log.getLogStream() << indent << "  " << *it << "\n";
  ft::log.writeEndl();

  ft::log.getLogStream() << indent << color << "[error_page]" END << std::endl;
  const ErrorPageType error_page = common.getErrorPage();
  for (ErrorPageType::const_iterator it = error_page.begin();
       it != error_page.end(); ++it)
    ft::log.getLogStream() << indent << "  " << it->first << " -> "
                           << it->second << "\n";
  ft::log.writeEndl();
}

void Config::printListen(const Config::ListenListType& listen_list,
                         const std::string&            indent) {
  for (ListenListType::const_iterator it = listen_list.begin();
       it != listen_list.end(); ++it) {
    sockaddr_in addr;
    addr.sin_addr.s_addr = it->first;
    ft::log.getLogStream() << indent << inet_ntoa(addr.sin_addr) << ":"
                           << it->second << "\n";
  }
  ft::log.writeEndl();
}
