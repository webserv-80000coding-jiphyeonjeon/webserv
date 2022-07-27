#include "ConfigServer.hpp"

ConfigServer::ConfigServer() : server_name_(), listen_(), location_(), common_config_() {}

ConfigServer::ConfigServer(const ConfigServer& other)
    : server_name_(other.server_name_),
      listen_(other.listen_),
      location_(other.location_),
      common_config_(other.common_config_) {}

ConfigServer::~ConfigServer() {}

ConfigServer& ConfigServer::operator=(const ConfigServer& other) {
  server_name_ = other.server_name_;
  listen_ = other.listen_;
  location_ = other.location_;
  common_config_ = other.common_config_;
  return *this;
}

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

void ConfigServer::setServerName(const ConfigServer::ServerNameType& server_name) {
  server_name_ = server_name;
}

void ConfigServer::addServerName(const std::string& value) { server_name_.push_back(value); }

void ConfigServer::setListen(const ConfigServer::ListenListType& listen) { listen_ = listen; }

void ConfigServer::addListen(const ConfigServer::ListenType& value) { listen_.push_back(value); }
