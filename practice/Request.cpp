#include "Request.hpp"

#include <iostream>
#include <map>
#include <string>
#include <vector>

namespace ft {
std::vector<std::string> split_by_string(std::string str,
                                         std::string delimiter);
std::string split_until_delimiter(std::string& str, std::string delimiter);
std::string get_until_delimiter(std::string& str, std::string delimiter,
                                size_t& pos);
}  // namespace ft

Request::Request() {}
Request::~Request() {}

// getter
std::string Request::getMethod() { return method_; }
std::string Request::getPath() { return path_; }
std::string Request::getVersion() { return version_; }
std::string Request::getHeader(std::string key) { return headers_[key]; }
std::string Request::getBody() { return body_; }
std::map<std::string, std::string> Request::getHeaders() { return headers_; }
Request::Flag                      Request::getState() { return state_; }
Request::Flag Request::getHeaderFlag() { return header_flag_; }
Request::Flag Request::getTransferFlag() { return transfer_flag_; }

// setter
void Request::setMethod(std::string method) { method_ = method; }
void Request::setPath(std::string path) { path_ = path; }
void Request::setVersion(std::string version) { version_ = version; }
void Request::setHeader(std::string key, std::string value) {
  headers_[key] = value;
}
void Request::setBody(std::string body) { body_ = body; }

void Request::concatenate_request(std::string request_message) {
  request_message_ += request_message;
}

// parsing
void Request::parse(std::string request_message) {
  size_t pos = 0;

  parse_start_line(ft::get_until_delimiter(request_message, "\r\n", pos));
  parse_header(request_message, pos);
  parse_body(request_message, pos);
}

void Request::parse_start_line(std::string start_line) {
  size_t pos = 0;

  method_ = ft::get_until_delimiter(start_line, " ", pos);
  path_ = ft::get_until_delimiter(start_line, " ", pos);
  version_ = start_line.erase(0, pos);
}

void Request::parse_header(std::string& request_message, size_t& pos) {
  std::string header_field;
  std::string field_name;

  while ((header_field =
              ft::get_until_delimiter(request_message, "\r\n", pos)) != "") {
    field_name = ft::split_until_delimiter(header_field, ":");
    headers_[field_name] = header_field;
  }
}
void Request::parse_body(std::string& request_message, size_t& pos) {
  body_ = request_message.erase(0, pos - 1);
}

// print
void Request::print() {
  typedef std::map<std::string, std::string>::iterator iterator;
  // status code
  std::cout << "====== status code ========" << std::endl;
  std::cout << method_ << " " << path_ << " " << version_ << std::endl;
  // headers
  std::cout << "====== headers ========" << std::endl;
  for (iterator it = headers_.begin(); it != headers_.end(); it++)
    std::cout << (*it).first << ": " << (*it).second << std::endl;
  // body
  std::cout << "====== body ========" << std::endl;
  std::cout << "\n" << body_ << std::endl;
}
