// Copyright (c) 2022 hyojekim. All rights reserved.

#ifndef CONFIGSERVER_HPP_
#define CONFIGSERVER_HPP_

#include "ConfigLocation.hpp"

class ConfigServer {
 public:
  typedef std::vector<std::string>              ServerNameType;
  typedef std::uint32_t                         AddressType;
  typedef std::uint16_t                         PortType;
  typedef std::pair<AddressType, PortType>      ListenType;
  typedef std::vector<ListenType>               ListenListType;
  typedef std::map<std::string, ConfigLocation> LocationType;
  typedef ConfigCommon                          Common;
  typedef Common::AutoindexType                 AutoindexType;
  typedef Common::ClientBodyBufferSizeType      ClientBodyBufferSizeType;
  typedef Common::ErrorCodeType                 ErrorCodeType;
  typedef Common::ErrorUriType                  ErrorUriType;
  typedef Common::ErrorPageType                 ErrorPageType;
  typedef Common::IndexFileType                 IndexFileType;
  typedef Common::IndexType                     IndexType;
  typedef Common::RootType                      RootType;

  ConfigServer();
  ConfigServer(const ConfigServer& other);
  ~ConfigServer();
  ConfigServer& operator=(const ConfigServer& other);

  const ServerNameType& getServerName() const;
  const ListenListType& getListen() const;
  const LocationType&   getLocation() const;
  const Common&         getCommon() const;

  const AutoindexType&            getAutoindexType() const;
  const ClientBodyBufferSizeType& getClientBodyBufferSize() const;
  const ErrorPageType&            getErrorPage() const;
  const IndexType&                getIndex() const;
  const RootType&                 getRoot() const;

 private:
  ServerNameType server_name_;
  ListenListType listen_;
  LocationType   location_;
  Common         common_config_;
};

#endif  // CONFIGSERVER_HPP_