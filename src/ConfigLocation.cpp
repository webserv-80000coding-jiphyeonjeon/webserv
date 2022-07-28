#include "ConfigLocation.hpp"

ConfigLocation::ConfigLocation() : method_set_(), limit_except_(), common_config_() {
  initMethodSet();
}

ConfigLocation::ConfigLocation(const ConfigLocation& other)
    : method_set_(other.method_set_),
      limit_except_(other.limit_except_),
      common_config_(other.common_config_) {}

ConfigLocation::~ConfigLocation() {}

ConfigLocation& ConfigLocation::operator=(const ConfigLocation& other) {
  method_set_ = other.method_set_;
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

void ConfigLocation::addLimitExcept(const ConfigLocation::MethodType& value) {
  limit_except_.insert(value);
}

void ConfigLocation::setCommon(const ConfigLocation::Common& common) { common_config_ = common; }

void ConfigLocation::setAutoindex(const ConfigLocation::AutoindexType& value) {
  common_config_.setAutoindex(value);
}

void ConfigLocation::setClientBodyBufferSize(
    const ConfigLocation::ClientBodyBufferSizeType& value) {
  common_config_.setClientBodyBufferSize(value);
}

void ConfigLocation::addErrorPage(const ConfigLocation::ErrorPairType& value) {
  common_config_.addErrorPage(value);
}

void ConfigLocation::setIndex(const ConfigLocation::IndexType& index) {
  common_config_.setIndex(index);
}

void ConfigLocation::addIndex(const ConfigLocation::IndexFileType& value) {
  common_config_.addIndex(value);
}

void ConfigLocation::setRoot(const ConfigLocation::RootType& value) {
  common_config_.setRoot(value);
}

bool ConfigLocation::isInMethodSet(const ConfigLocation::MethodType& method) const {
  return method_set_.find(method) != method_set_.end();
}

void ConfigLocation::initMethodSet() {
  method_set_.insert("GET");
  method_set_.insert("HEAD");
  method_set_.insert("POST");
  method_set_.insert("PUT");
  method_set_.insert("DELETE");
}
