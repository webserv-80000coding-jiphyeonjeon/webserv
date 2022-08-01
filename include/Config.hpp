// Copyright (c) 2022 hyojekim. All rights reserved.

#ifndef CONFIG_HPP_
#define CONFIG_HPP_

#include "ConfigCommon.hpp"
#include "ConfigLocation.hpp"
#include "ConfigServer.hpp"

class Config {
 public:
  typedef std::vector<ConfigServer>       ServersType;
  typedef ConfigServer::ServerNameType    ServerNameType;
  typedef ConfigServer::ListenListType    ListenListType;
  typedef ConfigServer::LocationType      LocationType;
  typedef ConfigServer::IndexType         IndexType;
  typedef ConfigServer::ErrorPageType     ErrorPageType;
  typedef ConfigLocation::LimitExceptType LimitExceptType;

  Config();
  ~Config();

  const ServersType& getServers() const;
  void               addServers(const ConfigServer& server);

  void printConfig() const;

 private:
  ServersType servers_;

  void printServer(const ConfigServer& server) const;
  void printLocation(const ConfigLocation& location,
                     const std::string&    indent) const;
  void printCommon(const ConfigCommon& common, const std::string& indent,
                   const std::string& color) const;
};

#endif  // CONFIG_HPP_
