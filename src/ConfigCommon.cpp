#include "ConfigCommon.hpp"

ConfigCommon::ConfigCommon()
    : autoindex_(false), client_body_buffer_size_(0), error_page_(), index_(), root_("") {}

ConfigCommon::ConfigCommon(const ConfigCommon& other)
    : autoindex_(other.autoindex_),
      client_body_buffer_size_(other.client_body_buffer_size_),
      error_page_(other.error_page_),
      index_(other.index_),
      root_(other.root_) {}

ConfigCommon::~ConfigCommon() {}

ConfigCommon& ConfigCommon::operator=(const ConfigCommon& other) {
  autoindex_ = other.autoindex_;
  client_body_buffer_size_ = other.client_body_buffer_size_;
  error_page_ = other.error_page_;
  index_ = other.index_;
  root_ = other.root_;
  return *this;
}

const ConfigCommon::AutoindexType& ConfigCommon::getAutoindexType() const { return autoindex_; }

const ConfigCommon::ClientBodyBufferSizeType& ConfigCommon::getClientBodyBufferSize() const {
  return client_body_buffer_size_;
}

const ConfigCommon::ErrorPageType& ConfigCommon::getErrorPage() const { return error_page_; }

const ConfigCommon::IndexType& ConfigCommon::getIndex() const { return index_; }

const ConfigCommon::RootType& ConfigCommon::getRoot() const { return root_; }

void ConfigCommon::setAutoindexType(const ConfigCommon::AutoindexType& value) {
  autoindex_ = value;
}

void ConfigCommon::setClientBodyBufferSize(const ConfigCommon::ClientBodyBufferSizeType& value) {
  client_body_buffer_size_ = value;
}

void ConfigCommon::addErrorPage(const ConfigCommon::ErrorPairType& value) {
  error_page_.insert(value);
}

void ConfigCommon::addIndex(const ConfigCommon::IndexFileType& value) { index_.push_back(value); }

void ConfigCommon::setRoot(const ConfigCommon::RootType& value) { root_ = value; }
