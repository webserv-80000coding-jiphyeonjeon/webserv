#include "Request.hpp"

#include <iostream>

#include "Utilities.hpp"

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

const RequestHeader::HeaderValueType& RequestHeader::getHeader(
    const RequestHeader::HeaderKeyType& key) const {
  return header_map_.at(key);
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

void RequestHeader::setHeader(const HeaderKeyType&   key,
                              const HeaderValueType& value) {
  header_map_[key] = value;
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

RequestHeader::ParseFuncMapType RequestHeader::initParseFuncMap() {
  ParseFuncMapType parse_func_map;

  parse_func_map["Host"] = &RequestHeader::parseHost;
  parse_func_map["Content-Length"] = &RequestHeader::parseContentLength;
  parse_func_map["Content-Type"] = &RequestHeader::parseContentType;
  parse_func_map["Transfer-Encoding"] = &RequestHeader::parseTransferCoding;

  return parse_func_map;
}

// ================================================================
// Request
// ================================================================

Request::Request()
    : method_(kNone),
      version_("HTTP/1.1"),
      state_(kContinuous),
      level_(kStartLine),
      position_(0) {}
Request::~Request() {}

const Request::MessageType& Request::getRequestMessage() const {
  return request_message_;
}

const Method& Request::getMethod() const { return method_; }

const Request::HeaderValueType& Request::getHeader(
    const HeaderKeyType& key) const {
  return header_.getHeader(key);
}

const Request::HeaderMapType& Request::getHeaderMap() const {
  return header_.getHeaderMap();
}

const Request::PathType& Request::getPath() const { return path_; }

const Request::VersionType& Request::getVersion() const { return version_; }

const Request::BodyType& Request::getBody() const { return body_; }

const State& Request::getState() const { return state_; }

void Request::setRequestMessage(const MessageType& request_message) {
  request_message_ = request_message;
}

void Request::setMethod(const Method& method) { method_ = method; }

void Request::setHeader(const HeaderKeyType&   key,
                        const HeaderValueType& value) {
  header_.setHeader(key, value);
}

void Request::setPath(const PathType& path) { path_ = path; }

void Request::setVersion(const VersionType& version) { version_ = version; }

void Request::setBody(const BodyType& body) { body_ = body; }

void Request::setState(const State& state) { state_ = state; }

int Request::parse(MessageType& request_message) {
  request_message_ += request_message;

  size_t pos;

  parseStartLine();
  parseHeader();
  parseBody();

  return state_;
}

void Request::parseStartLine() {
  std::string method;
  std::string methods[] = {"GET", "POST", "PUT", "DELETE", "HEAD"};

  if (level_ != kStartLine) return;
  if (request_message_.find("\r\n", position_) == std::string::npos) return;

  // TODO: Refactor below codes.
  // Method
  method = ft::getUntilDelimiter(request_message_, " ", position_);
  // No space or start from space.
  if (method == "") throw RequestException("Bad Request(method)", 400);
  for (int i = 0; i < 5; i++) {
    if (method == methods[i]) {
      method_ = static_cast<Method>(i + 1);
      break;
    }
  }
  // Method not supported.
  if (method_ == kNone) throw RequestException("Invalid method" + method, 501);

  // Path
  path_ = ft::getUntilDelimiter(request_message_, " ", position_);
  // No space or start from space.
  if (path_ == "") throw RequestException("Bad Request(path)", 400);
  // Path not start with '/'(means refused).
  if (path_[0] != '/') throw RequestException("Invalid path", 403);
  // Path is too long.(limit 2MB(2048))
  if (path_.length() > MAXIMUM_URI_LIMIT)
    throw RequestException("Too long URI", 414);
  // check is path_ valid(depth)
  // check is query_string valid

  // Version
  VersionType version =
      ft::getUntilDelimiter(request_message_, "\r\n", position_);
  // Webserv only support HTTP/1.1.
  if (version_ != version) throw RequestException("Invalid version", 505);

  // Next level.
  level_ = kHeader;
}

void Request::parseHeader() {
  PositionType    cp_pos = position_;
  HeaderKeyType   key;
  HeaderValueType value;

  // Content-Length: 0\r\n
  // Host: localhost:8080\r\n\r\n
  if (level_ != kHeader) return;
  while (request_message_.find("\r\n", position_) != std::string::npos) {
    std::string header =
        ft::getUntilDelimiter(request_message_, "\r\n", position_);
    // \r\n으로 시작하는 경우, header가 비어있으므로 400
    // \r\n\r\n인 경우, kBody레벨으로
    if (header == "") break;

    key = ft::splitUntilDelimiter(header, ": ");
    if (key == "") throw RequestException("Bad Request(header_key)", 400);
    if (getHeaderMap().find(key) != getHeaderMap().end()) {
      throw RequestException("Bad Request(duplicated header)", 400);
    }

    value = ft::strBidirectionalTrim(header, " ");
    if (value == "") throw RequestException("Bad Request(header_value)", 400);

    setHeader(key, value);
  }
  if (request_message_.find("\r\n\r\n", cp_pos) == std::string::npos)
    level_ = kBody;
}

void Request::parseBody() {}

void Request::print() const {
  std::cout << "Request" << std::endl;
  std::cout << "Method: " << getMethod() << std::endl;
  std::cout << "Path: " << getPath() << std::endl;
  std::cout << "Version: " << getVersion() << std::endl;
  std::cout << "Header: " << std::endl;
  for (HeaderMapType::const_iterator it = getHeaderMap().begin();
       it != getHeaderMap().end(); ++it) {
    std::cout << it->first << ": " << it->second << std::endl;
  }
  std::cout << "Body: " << getBody() << std::endl;
}