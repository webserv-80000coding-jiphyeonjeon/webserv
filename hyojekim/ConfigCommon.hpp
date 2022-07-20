// Copyright (c) 2022 hyojekim. All rights reserved.

#ifndef CONFIGCOMMON_HPP_
#define CONFIGCOMMON_HPP_
#pragma once

#include <map>
#include <string>
#include <vector>

class ConfigCommon {
 public:
  typedef bool                                      autoindex_type;
  typedef unsigned int                              client_body_buffer_size_type;
  typedef unsigned int                              error_code_type;
  typedef std::string                               error_uri_type;
  typedef std::map<error_code_type, error_uri_type> error_page_type;
  typedef std::string                               index_file_type;
  typedef std::vector<index_file_type>              index_type;
  typedef std::string                               root_type;

 private:
  autoindex_type               autoindex_;
  client_body_buffer_size_type client_body_buffer_size_;
  error_page_type              error_page_;
  index_type                   index_;
  root_type                    root_;
};

#endif  // CONFIGCOMMON_HPP_