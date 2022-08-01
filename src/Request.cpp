#include "Request.hpp"

// ================================================================
// RequestHeader
// ================================================================

RequestHeader::RequestHeader()
    : port_(0),
      content_length_(0),
      content_type_(0),
      transfer_coding_(kDefault) {
  parse_func_map_ = initParseFuncMap();
}

RequestHeader::~RequestHeader() {}

const RequestHeader::HeaderMapType& RequestHeader::getHeaderMap() const {
  return header_map_;
}

const RequestHeader::PortType& RequestHeader::getPort() const { return port_; }

const RequestHeader::ContentLengthType& RequestHeader::getContentLength()
    const {
  return content_length_;
}

const RequestHeader::ContentTypeType& RequestHeader::getContentType() const {
  return content_type_;
}

const TransferCoding& RequestHeader::getTransferCoding() const {
  return transfer_coding_;
}

void RequestHeader::setHeaderMap(const HeaderMapType& header_map) {
  header_map_ = header_map;
}

void RequestHeader::setPort(const PortType& port) { port_ = port; }

void RequestHeader::setContentLength(const ContentLengthType& content_length) {
  content_length_ = content_length;
}

void RequestHeader::setContentType(const ContentTypeType& content_type) {
  content_type_ = content_type;
}

void RequestHeader::setTransferCoding(const TransferCoding& transfer_coding) {
  transfer_coding_ = transfer_coding;
}

void RequestHeader::parseHost(Method method, HeaderValueType value) {
  (void)method;
  size_t pos;

  if ((pos = value.find(':')) != std::string::npos) {
    port_ = atoi(value.substr(pos + 1).c_str());
  } else {
    port_ = 80;
  }
}

void RequestHeader::parseContentLength(Method method, HeaderValueType value) {
  if (method == kPost)
    content_length_ = atoi(value.c_str());
  else
    content_length_ = 0;
}

void RequestHeader::parseContentType(Method method, HeaderValueType value) {
  (void)method;
  (void)value;
}

void RequestHeader::parseTransferCoding(Method method, HeaderValueType value) {
  (void)method;

  if (value == "chunked")
    transfer_coding_ = kChunked;
  else
    transfer_coding_ = kDefault;
}

// ================================================================
// Request
// ================================================================

Request::Request() : version_("HTTP/1.1") {}