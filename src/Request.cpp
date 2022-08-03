#include "Request.hpp"

#include <cstdlib>
#include <iostream>

#include "Utilities.hpp"

// ANCHOR: RequestHeader

RequestHeader::RequestHeader()
    : port_(0),
      content_length_(-1),
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

const RequestHeader::AddressType& RequestHeader::getAddress() const {
  return address_;
}

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

void RequestHeader::setHeaderMap(const HeaderMapType& header_map,
                                 const Method&        method) {
  header_map_.clear();
  for (HeaderMapType::const_iterator it = header_map.begin();
       it != header_map.end(); ++it) {
    setHeader(it->first, it->second, method);
  }
}

void RequestHeader::setHeader(const HeaderKeyType&   key,
                              const HeaderValueType& value,
                              const Method&          method) {
  header_map_[key] = value;

  if (parse_func_map_.find(key) != parse_func_map_.end())
    (this->*parse_func_map_[key])(method, value);
}

void RequestHeader::setAddress(const AddressType& address) {
  address_ = address;
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

  if (value.find(':') != std::string::npos) {
    address_ = ft::splitUntilDelimiter(value, ":");
    port_ = atoi(value.c_str());
  } else {
    // TODO: 나중에 Host IP와 동일한 지 확인
    address_ = value;
    port_ = 80;
  }
}

void RequestHeader::parseContentLength(Method method, HeaderValueType value) {
  if (method == kPost)
    content_length_ = atoi(value.c_str());
  else
    content_length_ = 0;
}

// TODO: parse content type
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

bool RequestHeader::isChunked() const { return transfer_coding_ == kChunked; }

RequestHeader::ParseFuncMapType RequestHeader::initParseFuncMap() {
  ParseFuncMapType parse_func_map;

  parse_func_map["Host"] = &RequestHeader::parseHost;
  parse_func_map["Content-Length"] = &RequestHeader::parseContentLength;
  parse_func_map["Content-Type"] = &RequestHeader::parseContentType;
  parse_func_map["Transfer-Encoding"] = &RequestHeader::parseTransferCoding;

  return parse_func_map;
}

// ANCHOR: Request

Request::Request()
    : method_(kNone),
      version_("HTTP/1.1"),
      state_(kContinuous),
      level_(kStartLine),
      position_(0),
      chunk_level_(kChunkSize),
      chunk_size_(0) {}
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
  header_.setHeader(key, value, method_);
}

void Request::setPath(const PathType& path) { path_ = path; }

void Request::setVersion(const VersionType& version) { version_ = version; }

void Request::setBody(const BodyType& body) { body_ = body; }

void Request::setState(const State& state) { state_ = state; }

int Request::parse(MessageType& request_message) {
  request_message_ += request_message;

  if (request_message_.size() > MAXIMUM_PAYLOAD_LIMIT)
    throw RequestException("Request payload is too large", 413);

  parseStartLine();
  parseHeader();
  parseBody();

  return state_;
}

void Request::parseStartLine() {
  std::string method;
  std::string methods[] = {"GET", "POST", "PUT", "DELETE", "HEAD"};

  if (level_ != kStartLine)
    return;
  if (request_message_.find("\r\n", position_) == std::string::npos)
    return;

  // TODO: Refactor below codes.
  // Method
  method = ft::getUntilDelimiter(request_message_, " ", position_);
  // No space or start from space.
  if (method == "")
    throw RequestException("Bad Request(method)", 400);
  for (int i = 0; i < 5; i++) {
    if (method == methods[i]) {
      method_ = static_cast<Method>(i + 1);
      break;
    }
  }
  // Method not supported.
  if (method_ == kNone)
    throw RequestException("Invalid method" + method, 501);

  // Path
  path_ = ft::getUntilDelimiter(request_message_, " ", position_);
  // No space or start from space.
  if (path_ == "")
    throw RequestException("Bad Request(path)", 400);
  // Path not start with '/'(means refused).
  if (path_[0] != '/')
    throw RequestException("Invalid path", 403);
  // Path is too long.(limit 2MB(2048))
  if (path_.length() > MAXIMUM_URI_LIMIT)
    throw RequestException("Too long URI", 414);
  // check is path_ valid(depth)
  // check is query_string valid

  // Version
  VersionType version =
      ft::getUntilDelimiter(request_message_, "\r\n", position_);
  // Webserv only support HTTP/1.1.
  if (version_ != version)
    throw RequestException("Invalid version", 505);

  // Next level.
  level_ = kHeader;
}

void Request::parseHeader() {
  HeaderKeyType   key;
  HeaderValueType value;

  if (level_ != kHeader)
    return;
  // Wait for header end.
  if (request_message_.find("\r\n\r\n", position_) == std::string::npos)
    return;
  // If header start from CRLF, throw exception.
  if (request_message_.find("\r\n", position_) == 0)
    throw RequestException("Bad Request(header)", 400);
  while (request_message_.find("\r\n", position_) != std::string::npos) {
    std::string header =
        ft::getUntilDelimiter(request_message_, "\r\n", position_);

    // End of header.
    if (header == "")
      break;

    key = ft::splitUntilDelimiter(header, ": ");
    // No ':' in header or space included in key.
    if (key == "" || key.find(' ') != std::string::npos)
      throw RequestException("Bad Request(header_key)", 400);
    // Duplicated header.
    if (getHeaderMap().find(key) != getHeaderMap().end())
      throw RequestException("Bad Request(duplicated header)", 400);

    value = ft::strBidirectionalTrim(header, " ");
    // No value in header.
    if (value == "")
      throw RequestException("Bad Request(header_value)", 400);

    setHeader(key, value);
  }

  if (header_.getHeader("Host") == "")
    throw RequestException("Bad Request(no host)", 400);

  // Erase request_message_ from start to position_(until end of header).
  request_message_.erase(0, position_);
  position_ = 0;
  // Next level.
  if (!header_.isChunked() && header_.getContentLength() == -1 &&
      (method_ == kPost || method_ == kPut))
    throw RequestException("Bad Request(no content-length)", 400);
  else if (header_.getContentLength() >= 0 || header_.isChunked())
    level_ = kBody;
  else
    level_ = kDone;
}

void Request::parseBody() {
  if (level_ != kBody)
    return;

  if (header_.isChunked()) {
    parseChunkedBody();
  } else {
    parseDefaultBody();
  }
}

void Request::parseChunkedBody() {
  static ChunkSizeType recv_size;

  if (request_message_.find("\r\n", position_) == std::string::npos)
    return;

  while (request_message_.find("\r\n", position_) != std::string::npos) {
    std::string chunk_data =
        ft::getUntilDelimiter(request_message_, "\r\n", position_);

    if (chunk_level_ == kChunkSize) {
      chunk_size_ = ft::hexStringToInt(chunk_data);

      if (chunk_size_ == 0) {
        chunk_level_ = kChunkEnd;
      } else {
        chunk_level_ = kChunkData;
      }
    } else if (chunk_level_ == kChunkData) {
      body_ += chunk_data;
      recv_size += chunk_data.size();
      // TODO: Check when error occurs.(recv_size couldn't be match)
      if (recv_size == chunk_size_) {
        chunk_size_ = 0;
        chunk_level_ = kChunkSize;
        recv_size = 0;
      }
    } else if (chunk_level_ == kChunkEnd) {
      HeaderKeyType   key;
      HeaderValueType value;

      if (chunk_data == "") {
        level_ = kDone;
        break;
      }

      // TODO: Duplicated code with parseHeader().
      key = ft::splitUntilDelimiter(chunk_data, ": ");
      // No ':' in header or space included in key.
      if (key == "" || key.find(' ') != std::string::npos)
        throw RequestException("Bad Request(header_key)", 400);
      // Duplicated header.
      if (getHeaderMap().find(key) != getHeaderMap().end())
        throw RequestException("Bad Request(duplicated header)", 400);
      std::cout << key << std::endl;

      value = ft::strBidirectionalTrim(chunk_data, " ");
      // No value in header.
      if (value == "")
        throw RequestException("Bad Request(header_value)", 400);

      setHeader(key, value);
    }
  }
}

void Request::parseDefaultBody() {
  if (request_message_.size() == header_.getContentLength()) {
    body_ = request_message_;
    level_ = kDone;
  } else if (request_message_.size() > header_.getContentLength()) {
    std::cout << header_.getContentLength() << std::endl;
    throw RequestException("Bad Request(Body longer than content-length)", 400);
  }
  return;
}

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