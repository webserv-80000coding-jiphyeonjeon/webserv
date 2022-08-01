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

  const AutoindexType&            getAutoindex() const;
  const ClientBodyBufferSizeType& getClientBodyBufferSize() const;
  const ErrorPageType&            getErrorPage() const;
  const IndexType&                getIndex() const;
  const RootType&                 getRoot() const;

  void addLimitExcept(const MethodType& value);
  void setCommon(const Common& common);

  void setAutoindex(const AutoindexType& value);
  void setClientBodyBufferSize(const ClientBodyBufferSizeType& value);
  void addErrorPage(const ErrorPairType& value);
  void setIndex(const IndexType& index);
  void addIndex(const IndexFileType& value);
  void setRoot(const RootType& value);

  bool isInMethodSet(const MethodType& method) const;

 private:
  LimitExceptType method_set_;
  LimitExceptType limit_except_;
  Common          common_config_;

  void initMethodSet();
};

#endif  // CONFIGLOCATION_HPP_
