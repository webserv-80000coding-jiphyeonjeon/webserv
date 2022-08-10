#ifndef CONFIGLOCATION_HPP_
#define CONFIGLOCATION_HPP_

#include <set>

#include "ConfigCommon.hpp"

class ConfigLocation {
 public:
  typedef std::string                         MethodType;
  typedef std::pair<std::string, std::string> CgiPairType;
  typedef std::map<CgiPairType::first_type, CgiPairType::second_type> CgiType;
  typedef ConfigCommon                                                Common;
  typedef std::set<MethodType>             LimitExceptType;
  typedef Common::AutoindexType            AutoindexType;
  typedef Common::ClientBodyBufferSizeType ClientBodyBufferSizeType;
  typedef Common::ErrorCodeType            ErrorCodeType;
  typedef Common::ErrorUriType             ErrorUriType;
  typedef Common::ErrorPairType            ErrorPairType;
  typedef Common::ErrorPageType            ErrorPageType;
  typedef Common::IndexFileType            IndexFileType;
  typedef Common::IndexType                IndexType;
  typedef Common::ReturnCodeType           ReturnCodeType;
  typedef Common::ReturnUrlType            ReturnUrlType;
  typedef Common::ReturnType               ReturnType;
  typedef Common::RootType                 RootType;

  ConfigLocation();
  ConfigLocation(const ConfigLocation& other);
  ~ConfigLocation();
  ConfigLocation& operator=(const ConfigLocation& other);

  const CgiType&         getCgi() const;
  const LimitExceptType& getLimitExcept() const;
  const Common&          getCommon() const;

  const AutoindexType&            getAutoindex() const;
  const ClientBodyBufferSizeType& getClientBodyBufferSize() const;
  const ErrorPageType&            getErrorPage() const;
  const IndexType&                getIndex() const;
  const ReturnType&               getReturn() const;
  const RootType&                 getRoot() const;

  void addCgi(const CgiPairType& value);
  void addLimitExcept(const MethodType& value);
  void setCommon(const Common& common);

  void setAutoindex(const AutoindexType& value);
  void setClientBodyBufferSize(const ClientBodyBufferSizeType& value);
  void setErrorPage(const ErrorPageType& error_page);
  void addErrorPage(const ErrorPairType& value);
  void setIndex(const IndexType& index);
  void addIndex(const IndexFileType& value);
  void setReturn(const ReturnType& value);
  void setRoot(const RootType& value);

  bool isInMethodSet(const MethodType& method) const;

 private:
  LimitExceptType method_set_;
  CgiType         cgi_;
  LimitExceptType limit_except_;
  Common          common_config_;

  void initMethodSet();
};

#endif  // CONFIGLOCATION_HPP_
