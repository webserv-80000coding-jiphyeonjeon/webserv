#include "ConfigCommon.hpp"

ConfigCommon::ConfigCommon() {}
ConfigCommon::ConfigCommon(const ConfigCommon& other) {}
ConfigCommon::~ConfigCommon() {}
ConfigCommon& ConfigCommon::operator=(const ConfigCommon& other) {}

const ConfigCommon::AutoindexType& ConfigCommon::getAutoindexType() const { return autoindex_; }
const ConfigCommon::ClientBodyBufferSizeType& ConfigCommon::getClientBodyBufferSize() const {
  return client_body_buffer_size_;
}
const ConfigCommon::ErrorPageType& ConfigCommon::getErrorPage() const { return error_page_; }
const ConfigCommon::IndexType&     ConfigCommon::getIndex() const { return index_; }
const ConfigCommon::RootType&      ConfigCommon::getRoot() const { return root_; }
