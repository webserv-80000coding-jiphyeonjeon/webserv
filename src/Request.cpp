#include "Request.hpp"

#include <cstdlib>
#include <iostream>

#include "Log.hpp"
#include "Utilities.hpp"

// ANCHOR: RequestHeader

RequestHeader::RequestHeader()
    : port_(0),
      content_length_(0),
      content_type_(0),
      transfer_coding_(kDefault) {
  initParseFuncMap();
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

const RequestHeader::ServerNameType& RequestHeader::getServerName() const {
  return server_name_;
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

void RequestHeader::setServerName(const ServerNameType& server_name) {
  server_name_ = server_name;
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
  // size_t pos;

  if (value.find(':') != std::string::npos) {
    server_name_ = ft::splitUntilDelimiter(value, ":");
    port_ = atoi(value.c_str());
  } else {
    // TODO: 나중에 Host IP와 동일한 지 확인
    server_name_ = value;
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

void RequestHeader::initParseFuncMap() {
  parse_func_map_["Host"] = &RequestHeader::parseHost;
  parse_func_map_["Content-Length"] = &RequestHeader::parseContentLength;
  parse_func_map_["Content-Type"] = &RequestHeader::parseContentType;
  parse_func_map_["Transfer-Encoding"] = &RequestHeader::parseTransferCoding;
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

const Level& Request::getLevel() const { return level_; }

void Request::setRequestMessage(const MessageType& request_message) {
  request_message_ = request_message;
}

const RequestHeader::ServerNameType& Request::getServerName() const {
  return header_.getServerName();
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
  if (request_message == "\r\n" && level_ == kStartLine)
    return state_;
  request_message_ += request_message;

  ft::log.writeTimeLog("Parsing...");
  ft::log.writeLog(request_message_);

  if (request_message_.size() > MAXIMUM_PAYLOAD_LIMIT)
    throw RequestException("Request payload is too large", 413);

  parseStartLine();
  parseHeader();
  parseBody();

  return state_;
}

void Request::parseStartLine() {
  std::string                   method;
  std::map<std::string, Method> method_map;
  method_map["GET"] = kGet;
  method_map["POST"] = kPost;
  method_map["PUT"] = kPut;
  method_map["DELETE"] = kDelete;
  method_map["HEAD"] = kHead;

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
  // Method not supported.
  if (method_map.find(method) == method_map.end())
    throw RequestException("Invalid method: " + method, 400);

  method_ = method_map[method];

  // Path
  path_ = ft::getUntilDelimiter(request_message_, " ", position_);
  // No space or start from space.
  if (path_ == "")
    throw RequestException("Bad Request(path)", 400);
  // Path not start with '/'(means refused).
  if (path_[0] != '/')
    throw RequestException("Invalid path", 400);
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
    throw RequestException("Invalid version", 400);

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

  if (header_.getHeaderMap().find("Host") == header_.getHeaderMap().end())
    throw RequestException("Bad Request(no host)", 400);

  // Erase request_message_ from start to position_(until end of header).
  request_message_.erase(0, position_);
  position_ = 0;
  // Next level.
  const HeaderMapType& header_map = getHeaderMap();
  if (!header_.isChunked() &&
      header_map.find("Content-Length") == header_map.end() &&
      (method_ == kPost || method_ == kPut))
    throw RequestException("Bad Request(no content-length)", 405);
  else if (header_.getContentLength() > 0 || header_.isChunked())
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

// void Request::print() const {
//   std::cout << "Request" << std::endl;
//   std::cout << "Method: " << getMethod() << std::endl;
//   std::cout << "Path: " << getPath() << std::endl;
//   std::cout << "Version: " << getVersion() << std::endl;
//   std::cout << "Header: " << std::endl;
//   for (HeaderMapType::const_iterator it = getHeaderMap().begin();
//        it != getHeaderMap().end(); ++it) {
//     std::cout << it->first << ": " << it->second << std::endl;
//   }
//   std::cout << "Body: " << getBody() << std::endl;
// }

std::string Request::printToString() const {
  std::stringstream ss;
  ss << "\nRequest" << std::endl;
  ss << "Method: " << getMethod() << std::endl;
  ss << "Path: " << getPath() << std::endl;
  ss << "Version: " << getVersion() << std::endl;
  ss << "Header: " << std::endl;
  for (HeaderMapType::const_iterator it = getHeaderMap().begin();
       it != getHeaderMap().end(); ++it) {
    ss << it->first << ": " << it->second << std::endl;
  }
  ss << "Body: " << getBody() << std::endl;
  return ss.str();
}
