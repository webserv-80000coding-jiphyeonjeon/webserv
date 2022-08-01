#include "Request.hpp"

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
  if ((pos = request_message_.find("\r\n")) == std::string::npos)
    return kContinuous;
  // FIXME error
  parseStartLine(ft::getUntilDelimiter(request_message_, "\r\n", position_));
  parseHeader(request_message);
  parseBody(request_message);

  return state_;
}

void Request::parseStartLine(MessageType& start_line) {
  size_t      pos = position_;
  std::string method;
  std::string methods[] = {"GET", "POST", "PUT", "DELETE", "HEAD"};

  if (level_ != kStartLine) return;
  if ((pos = start_line.find("\r\n", position_)) == std::string::npos) return;

  method = ft::getUntilDelimiter(start_line, " ", pos);
  for (int i = 0; i < 5; i++) {
    if (method == methods[i]) {
      method_ = static_cast<Method>(i);
      break;
    }
  }

  path_ = ft::getUntilDelimiter(start_line, " ", pos);
}