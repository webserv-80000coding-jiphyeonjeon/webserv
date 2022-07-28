#include "Response.hpp"

#include <iostream>

#include "Utility.hpp"

std::map<int, std::string> Response::createStatusCodes() {
  std::map<int, std::string> status_code;
  status_code[200] = "OK";
  status_code[201] = "Created";
  status_code[204] = "No Content";
  status_code[205] = "Reset Content";
  status_code[301] = "Moved Permanently";
  status_code[303] = "See Other";
  status_code[307] = "Temporary Redirect";
  status_code[400] = "Bad Request";
  status_code[401] = "Unauthorized";
  status_code[403] = "Forbidden";
  status_code[404] = "Not Found";
  status_code[405] = "Method Not Allowed";
  status_code[408] = "Request Timeout";
  status_code[410] = "Gone";
  status_code[411] = "Length Required";
  status_code[413] = "Payload Too Large";
  status_code[414] = "URI Too Long";
  status_code[500] = "Internal Server Error";
  status_code[503] = "Service Unavailable";
  status_code[504] = "Gateway Timeout";
  status_code[505] = "HTTP Version Not Supported";

  return status_code;
}

const std::map<int, std::string> Response::kStatusCodes = createStatusCodes();

Response::Response() : version_("HTTP/1.1") {}
Response::~Response() {}

const std::string& Response::getResponseMessage() const {
  return response_message_;
}
const std::string& Response::getVersion() const { return version_; }
const int&         Response::getStatusCode() const { return status_code_; }
const Response::HeaderMap& Response::getHeaderMap() const { return headerMap_; }
const std::string&         Response::getHeader(const std::string& key) const {
          return headerMap_.at(key);
}
const std::string& Response::getBody() const { return body_; }

void Response::setVersion(std::string version) { version_ = version; }
void Response::setStatusCode(int status_code) { status_code_ = status_code; }
void Response::setHeaderMap(HeaderMap header_map) { headerMap_ = header_map; }
void Response::setHeader(const std::string& key, const std::string& value) {
  headerMap_[key] = value;
}
void Response::setBody(std::string body) { body_ = body; }

void Response::build() {
  buildStartLine();
  buildHeader();
  buildBody();
}

void Response::buildStartLine() {
  response_message_ = version_ + " " + ft::toString(status_code_) + " " +
                      kStatusCodes.at(status_code_) + "\r\n";
}

void Response::buildHeader() {
  for (HeaderMap::const_iterator it = headerMap_.begin();
       it != headerMap_.end(); ++it) {
    response_message_ += it->first + ": " + it->second + "\r\n";
  }
  response_message_ += "\r\n";
}

void Response::buildBody() { response_message_ += body_; }

void Response::print() { std::cout << response_message_ << std::endl; }