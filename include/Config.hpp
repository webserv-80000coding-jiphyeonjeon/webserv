// Copyright (c) 2022 hyojekim. All rights reserved.

#ifndef CONFIG_HPP_
#define CONFIG_HPP_

#include "ConfigCommon.hpp"
#include "ConfigLocation.hpp"
#include "ConfigServer.hpp"

class Config {
 public:
  typedef std::vector<ConfigServer>                 ServersType;
  typedef ConfigServer::ServerNameType              ServerNameType;
  typedef ConfigServer::ListenType                  ListenType;
  typedef ConfigServer::ListenListType              ListenListType;
  typedef ConfigServer::LocationType                LocationType;
  typedef ConfigServer::IndexType                   IndexType;
  typedef ConfigServer::ErrorPageType               ErrorPageType;
  typedef ConfigLocation::LimitExceptType           LimitExceptType;
  typedef std::map<ListenType, const ConfigServer*> ConfigFinderType;

  static void printServer(const ConfigServer& server);
  static void printLocation(const ConfigLocation& location,
                            const std::string&    indent);
  static void printCommon(const ConfigCommon& common, const std::string& indent,
                          const std::string& color);
  static void printListen(const ListenListType& listen_list,
                          const std::string&    indent);

  Config();
  ~Config();

  const ServersType& getServers() const;
  void               addServers(const ConfigServer& server);
  ListenListType     getAllListenList() const;
  ConfigFinderType   getConfigFinder() const;

  void printConfig() const;

 private:
  ServersType servers_;
};

#endif  // CONFIG_HPP_
