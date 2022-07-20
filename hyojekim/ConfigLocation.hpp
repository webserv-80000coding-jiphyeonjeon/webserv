// Copyright (c) 2022 hyojekim. All rights reserved.

#ifndef CONFIGLOCATION_HPP_
#define CONFIGLOCATION_HPP_
#pragma once

#include <set>

#include "ConfigCommon.hpp"

class ConfigLocation {
 public:
  typedef std::string                          method_type;
  typedef std::set<method_type>                limit_except_type;
  typedef ConfigCommon                         common;
  typedef common::autoindex_type               autoindex_type;
  typedef common::client_body_buffer_size_type client_body_buffer_size_type;
  typedef common::error_code_type              error_code_type;
  typedef common::error_uri_type               error_uri_type;
  typedef common::error_page_type              error_page_type;
  typedef common::index_file_type              index_file_type;
  typedef common::index_type                   index_type;
  typedef common::root_type                    root_type;
  // typedef something cgi

 private:
  limit_except_type limit_except_;
  common            common_config_;
};

#endif  // CONFIGLOCATION_HPP_