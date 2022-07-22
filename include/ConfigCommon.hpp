// Copyright (c) 2022 hyojekim. All rights reserved.

#ifndef CONFIGCOMMON_HPP_
#define CONFIGCOMMON_HPP_

#include <sys/types.h>

#include <map>
#include <string>
#include <vector>

class ConfigCommon {
 public:
  typedef bool                                  AutoindexType;
  typedef std::size_t                           ClientBodyBufferSizeType;
  typedef unsigned int                          ErrorCodeType;
  typedef std::string                           ErrorUriType;
  typedef std::map<ErrorCodeType, ErrorUriType> ErrorPageType;
  typedef std::string                           IndexFileType;
  typedef std::vector<IndexFileType>            IndexType;
  typedef std::string                           RootType;

  ConfigCommon();
  ConfigCommon(const ConfigCommon& other);
  ~ConfigCommon();
  ConfigCommon& operator=(const ConfigCommon& other);

  const AutoindexType            getAutoindexType() const;
  const ClientBodyBufferSizeType getClientBodyBufferSize() const;
  const ErrorCodeType            getErrorCode() const;
  const IndexType                getIndex() const;
  const RootType                 getRoot() const;

 private:
  AutoindexType            autoindex_;
  ClientBodyBufferSizeType client_body_buffer_size_;
  ErrorPageType            error_page_;
  IndexType                index_;
  RootType                 root_;
};

#endif  // CONFIGCOMMON_HPP_
