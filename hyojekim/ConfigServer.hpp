// Copyright (c) 2022 hyojekim. All rights reserved.

#ifndef CONFIGSERVER_HPP_
#define CONFIGSERVER_HPP_
#pragma once

#include "ConfigLocation.hpp"

class ConfigServer {
 public:
  typedef std::vector<std::string>              server_name_type;
  typedef unsigned long                         address_type;
  typedef unsigned int                          port_type;
  typedef std::pair<address_type, port_type>    listen_type;
  typedef std::vector<listen_type>              listen_list_type;
  typedef std::map<std::string, ConfigLocation> location_type;
  typedef ConfigCommon                          common;
  typedef common::autoindex_type                autoindex_type;
  typedef common::client_body_buffer_size_type  client_body_buffer_size_type;
  typedef common::error_code_type               error_code_type;
  typedef common::error_uri_type                error_uri_type;
  typedef common::error_page_type               error_page_type;
  typedef common::index_file_type               index_file_type;
  typedef common::index_type                    index_type;
  typedef common::root_type                     root_type;

 private:
  server_name_type server_name_;
  listen_list_type listen_;
  location_type    location_;
  common           common_config_;
};

#endif  // CONFIGSERVER_HPP_