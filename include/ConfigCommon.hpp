// Copyright (c) 2022 hyojekim. All rights reserved.

#ifndef CONFIGCOMMON_HPP_
#define CONFIGCOMMON_HPP_

#include <sys/types.h>

#include <map>
#include <string>
#include <vector>

class ConfigCommon {
 public:
  typedef bool                                   AutoindexType;
  typedef std::size_t                            ClientBodyBufferSizeType;
  typedef unsigned int                           ErrorCodeType;
  typedef std::string                            ErrorUriType;
  typedef std::pair<ErrorCodeType, ErrorUriType> ErrorPairType;
  typedef std::map<ErrorPairType::first_type, ErrorPairType::second_type>
                                                   ErrorPageType;
  typedef std::string                              IndexFileType;
  typedef std::vector<IndexFileType>               IndexType;
  typedef unsigned int                             ReturnCodeType;
  typedef std::string                              ReturnUrlType;
  typedef std::pair<ReturnCodeType, ReturnUrlType> ReturnType;
  typedef std::string                              RootType;

  ConfigCommon();
  ConfigCommon(const ConfigCommon& other);
  ~ConfigCommon();
  ConfigCommon& operator=(const ConfigCommon& other);

  const AutoindexType&            getAutoindex() const;
  const ClientBodyBufferSizeType& getClientBodyBufferSize() const;
  const ErrorPageType&            getErrorPage() const;
  const IndexType&                getIndex() const;
  const ReturnType&               getReturn() const;
  const RootType&                 getRoot() const;

  void setAutoindex(const AutoindexType& value);
  void setClientBodyBufferSize(const ClientBodyBufferSizeType& value);
  void addErrorPage(const ErrorPairType& value);
  void setIndex(const IndexType& index);
  void addIndex(const IndexFileType& value);
  void setReturn(const ReturnType& value);
  void setRoot(const RootType& value);

 private:
  static const AutoindexType            kDefaultAutoindex = false;
  static const ClientBodyBufferSizeType kDefaultClientBodyBufferSize =
      100000001;

  AutoindexType            autoindex_;
  ClientBodyBufferSizeType client_body_buffer_size_;
  ErrorPageType            error_page_;
  IndexType                index_;
  ReturnType               return_;
  RootType                 root_;
};

#endif  // CONFIGCOMMON_HPP_
