#ifndef CONFIGSERVER_HPP_
#define CONFIGSERVER_HPP_

#include "ConfigLocation.hpp"

class ConfigServer {
 public:
  typedef std::vector<std::string>               ServerNamesType;
  typedef std::uint32_t                          AddressType;
  typedef std::uint16_t                          PortType;
  typedef std::pair<AddressType, PortType>       ListenType;
  typedef std::vector<ListenType>                ListenListType;
  typedef std::pair<std::string, ConfigLocation> LocationPairType;
  typedef std::map<LocationPairType::first_type, LocationPairType::second_type>
                                           LocationType;
  typedef ConfigCommon                     Common;
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

  ConfigServer();
  ConfigServer(const ConfigServer& other);
  ~ConfigServer();
  ConfigServer& operator=(const ConfigServer& other);

  const ServerNamesType& getServerName() const;
  const ListenListType&  getListen() const;
  const LocationType&    getLocation() const;
  const Common&          getCommon() const;

  const AutoindexType&            getAutoindex() const;
  const ClientBodyBufferSizeType& getClientBodyBufferSize() const;
  const ErrorPageType&            getErrorPage() const;
  const IndexType&                getIndex() const;
  const ReturnType&               getReturn() const;
  const RootType&                 getRoot() const;

  void setServerName(const ServerNamesType& server_name);
  void addServerName(const std::string& value);
  void setListen(const ListenListType& listen);
  void addListen(const ListenType& value);

  void setLocation(const LocationType& location);
  void addLocation(const LocationPairType& value);
  void setCommon(const Common& common);

  void setAutoindex(const AutoindexType& value);
  void setClientBodyBufferSize(const ClientBodyBufferSizeType& value);
  void addErrorPage(const ErrorPairType& value);
  void addIndex(const IndexFileType& value);
  void setReturn(const ReturnType& value);
  void setRoot(const RootType& value);

 private:
  ServerNamesType server_name_;
  ListenListType  listen_;
  LocationType    location_;
  Common          common_config_;
};

#endif  // CONFIGSERVER_HPP_
