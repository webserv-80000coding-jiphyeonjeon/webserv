#include "ConfigLocation.hpp"

ConfigLocation::ConfigLocation() : limit_except_(), common_config_() {}

ConfigLocation::ConfigLocation(const ConfigLocation& other)
    : limit_except_(other.limit_except_), common_config_(other.common_config_) {}

ConfigLocation::~ConfigLocation() {}

ConfigLocation& ConfigLocation::operator=(const ConfigLocation& other) {
  limit_except_ = other.limit_except_;
  common_config_ = other.common_config_;
  return *this;
}

const ConfigLocation::LimitExceptType& ConfigLocation::getLimitExcept() const {
  return limit_except_;
}

const ConfigLocation::Common& ConfigLocation::getCommon() const { return common_config_; }

const ConfigLocation::AutoindexType& ConfigLocation::getAutoindex() const {
  return common_config_.getAutoindex();
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
