#include "RequestHeader.hpp"

RequestHeader::RequestHeader()
    : headerParseFuncMap_(createHeaderParseFuncMap()) {
  headers_ = HeaderMap();
  host_port_ = 0;
  content_length_ = 0;
  content_type_ = 0;
  transfer_coding_ = 0;
}

RequestHeader::~RequestHeader() {}

RequestHeader::HeaderParseFuncMap RequestHeader::createHeaderParseFuncMap() {
  HeaderParseFuncMap header_parse_func_map;

  header_parse_func_map["Host"] = &RequestHeader::parseHost;
  header_parse_func_map["Content-Length"] = &RequestHeader::parseContentLength;
  header_parse_func_map["Content-Type"] = &RequestHeader::parseContentType;
  header_parse_func_map["Transfer-Encoding"] =
      &RequestHeader::parseTransferEncoding;

  return header_parse_func_map;
}

const RequestHeader::HeaderMap &RequestHeader::getHeaders() { return headers_; }
const std::string              &RequestHeader::getHeader(std::string key) {
               return headers_[key];
}
const int &RequestHeader::getHostPort() { return host_port_; }
const int &RequestHeader::getContentLength() { return content_length_; }
const RequestHeader::Flag &RequestHeader::getContentType() {
  return content_type_;
}
const RequestHeader::Flag &RequestHeader::getTransferEncoding() {
  return transfer_coding_;
}

void RequestHeader::setHeaders(HeaderMap headers) {
  for (int i = 0; i < headers.size(); i++) {
    setHeader(headers.begin()->first, headers.begin()->second, kNone);
  }
}

void RequestHeader::setHeader(std::string key, std::string value, Flag method) {
  if (headers_.find(key) != headers_.end()) {
    ;  // error
  } else if (headerParseFuncMap_.find(key) != headerParseFuncMap_.end()) {
    (this->*headerParseFuncMap_[key])(method, value);
  }
  headers_[key] = value;
}

void RequestHeader::setHostPort(int host_port) { host_port_ = host_port; }
void RequestHeader::setContentLength(int content_length) {
  content_length_ = content_length;
}
void RequestHeader::setContentType(Flag content_type) {
  content_type_ = content_type;
}
void RequestHeader::setTransferEncoding(Flag transfer_encoding) {
  transfer_coding_ = transfer_encoding;
}

void RequestHeader::turnOnContentTypeFlag(int flag) {
  content_type_ = content_type_ | flag;
}
void RequestHeader::turnOnTransferEncodingFlag(int flag) {
  transfer_coding_ = transfer_coding_ | flag;
}

void RequestHeader::parseHost(Flag method, std::string value) {
  (void)method;
  size_t pos;

  if ((pos = headers_["Host"].find(':')) != std::string::npos) {
    host_port_ = atoi(headers_["Host"].substr(pos + 1).c_str());
  } else {
    host_port_ = 80;
  }
}

void RequestHeader::parseContentLength(Flag method, std::string value) {
  if (method == kPost)
    content_length_ = atoi(value.c_str());
  else if (method != kPost && value != "0")
    ;  // error
}

void RequestHeader::parseContentType(Flag method, std::string value) {
  (void)method;
  (void)value;
}

void RequestHeader::parseTransferEncoding(Flag method, std::string value) {
  (void)method;
  if (value == "chunked")
    transfer_coding_ = kChunked;
  else
    transfer_coding_ = kDefault;
}