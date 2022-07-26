// Copyright (c) 2022 hyojekim. All rights reserved.

#ifndef CONFIGLOCATION_HPP_
#define CONFIGLOCATION_HPP_

#include <set>

#include "ConfigCommon.hpp"

class ConfigLocation {
 public:
  typedef std::string                      MethodType;
  typedef std::set<MethodType>             LimitExceptType;
  typedef ConfigCommon                     Common;
  typedef Common::AutoindexType            AutoindexType;
  typedef Common::ClientBodyBufferSizeType ClientBodyBufferSizeType;
  typedef Common::ErrorCodeType            ErrorCodeType;
  typedef Common::ErrorUriType             ErrorUriType;
  typedef Common::ErrorPairType            ErrorPairType;
  typedef Common::ErrorPageType            ErrorPageType;
  typedef Common::IndexFileType            IndexFileType;
  typedef Common::IndexType                IndexType;
  typedef Common::RootType                 RootType;
  // typedef something cgi

  ConfigLocation();
  ConfigLocation(const ConfigLocation& other);
  ~ConfigLocation();
  ConfigLocation& operator=(const ConfigLocation& other);

  const LimitExceptType& getLimitExcept() const;
  const Common&          getCommon() const;

  const AutoindexType&            getAutoindexType() const;
  const ClientBodyBufferSizeType& getClientBodyBufferSize() const;
  const ErrorPageType&            getErrorPage() const;
  const IndexType&                getIndex() const;
  const RootType&                 getRoot() const;

  void setAutoindexType(const AutoindexType& value);
  void setClientBodyBufferSize(const ClientBodyBufferSizeType& value);
  void addErrorPage(const ErrorPairType& value);
  void addIndex(const IndexFileType& value);
  void setRoot(const RootType& value);

 private:
  LimitExceptType limit_except_;
  Common          common_config_;
};

#endif  // CONFIGLOCATION_HPP_
