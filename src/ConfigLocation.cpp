#include "ConfigLocation.hpp"

ConfigLocation::ConfigLocation() {}
ConfigLocation::ConfigLocation(const ConfigLocation& other) {}
ConfigLocation::~ConfigLocation() {}
ConfigLocation& ConfigLocation::operator=(const ConfigLocation& other) {}

const ConfigLocation::LimitExceptType& ConfigLocation::getLimitExcept() const {
  return limit_except_;
}

const ConfigLocation::Common& ConfigLocation::getCommon() const { return common_config_; }

const ConfigLocation::AutoindexType& ConfigLocation::getAutoindexType() const {
  return common_config_.getAutoindexType();
}

const ConfigLocation::ClientBodyBufferSizeType& ConfigLocation::getClientBodyBufferSize() const {
  return common_config_.getClientBodyBufferSize();
}

const ConfigLocation::ErrorPageType& ConfigLocation::getErrorPage() const {
  return common_config_.getErrorPage();
}

const ConfigLocation::IndexType& ConfigLocation::getIndex() const {
  return common_config_.getIndex();
}

const ConfigLocation::RootType& ConfigLocation::getRoot() const { return common_config_.getRoot(); }
