// Copyright (c) 2022 hyojekim. All rights reserved.

#ifndef CONFIG_HPP_
#define CONFIG_HPP_

#include "ConfigCommon.hpp"
#include "ConfigLocation.hpp"
#include "ConfigServer.hpp"

class Config {
 public:
  typedef std::vector<std::string> ServerType;
  // typedef std::vector<ConfigServer> ServerType;

  Config();
  ~Config();

  const ServerType& getServers() const;
  void              setServers(const ServerType& servers);

 private:
  ServerType servers_;
};

// std::ostream& operator<<(std::ostream& os, const Config& config);

#endif  // CONFIG_HPP_
