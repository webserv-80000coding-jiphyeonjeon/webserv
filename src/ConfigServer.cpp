#include "ConfigServer.hpp"

#include "color.hpp"

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

const ConfigServer::AutoindexType& ConfigServer::getAutoindex() const {
  return common_config_.getAutoindex();
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

void ConfigServer::addListen(const ConfigServer::ListenType& value) {
  for (ListenListType::iterator it = listen_.begin(); it != listen_.end(); ++it) {
    if (*it == value)
      throw std::invalid_argument(RED "Config: listen: Duplicate host:port is not allowed." END);
  }

  listen_.push_back(value);
}

void ConfigServer::setLocation(const ConfigServer::LocationType& location) { location_ = location; }

void ConfigServer::addLocation(const ConfigServer::LocationPairType& value) {
  location_.insert(value);
}

void ConfigServer::setCommon(const ConfigServer::Common& common) { common_config_ = common; }

void ConfigServer::setAutoindex(const ConfigServer::AutoindexType& value) {
  common_config_.setAutoindex(value);
}

void ConfigServer::setClientBodyBufferSize(const ConfigServer::ClientBodyBufferSizeType& value) {
  common_config_.setClientBodyBufferSize(value);
}

void ConfigServer::addErrorPage(const ConfigServer::ErrorPairType& value) {
  common_config_.addErrorPage(value);
}

void ConfigServer::addIndex(const ConfigServer::IndexFileType& value) {
  common_config_.addIndex(value);
}

void ConfigServer::setRoot(const ConfigServer::RootType& value) { common_config_.setRoot(value); }
