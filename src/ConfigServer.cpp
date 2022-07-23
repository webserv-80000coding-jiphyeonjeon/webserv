#include "ConfigServer.hpp"

ConfigServer::ConfigServer() {}
ConfigServer::ConfigServer(const ConfigServer& other) {}
ConfigServer::~ConfigServer() {}
ConfigServer& ConfigServer::operator=(const ConfigServer& other) {}

const ConfigServer::ServerNameType& ConfigServer::getServerName() const { return server_name_; }

const ConfigServer::ListenListType& ConfigServer::getListen() const { return listen_; }

const ConfigServer::LocationType& ConfigServer::getLocation() const { return location_; }

const ConfigServer::Common& ConfigServer::getCommon() const { return common_config_; }

const ConfigServer::AutoindexType& ConfigServer::getAutoindexType() const {
  return common_config_.getAutoindexType();
}

const ConfigServer::ClientBodyBufferSizeType& ConfigServer::getClientBodyBufferSize() const {
  return common_config_.getClientBodyBufferSize();
}

const ConfigServer::ErrorPageType& ConfigServer::getErrorPage() const {
  return common_config_.getErrorPage();
}

const ConfigServer::IndexType& ConfigServer::getIndex() const { return common_config_.getIndex(); }

const ConfigServer::RootType& ConfigServer::getRoot() const { return common_config_.getRoot(); }