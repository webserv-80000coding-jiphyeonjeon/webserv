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
void        str_ltrim(std::string& str, const std::string& charset);
void        str_rtrim(std::string& str, const std::string& charset);
std::string str_bidirectional_trim(std::string&       str,
                                   const std::string& charset);
}  // namespace ft

Request::Request() {}
Request::~Request() {}

// getter
Request::Flag Request::getMethod() { return method_; }
std::string   Request::getPath() { return path_; }
std::string   Request::getVersion() { return version_; }
std::string   Request::getHeader(std::string key) {
    return header_.getHeader(key);
}
std::string   Request::getBody() { return body_; }
Request::Flag Request::getState() { return state_; }
Request::Flag Request::getHeaderFlag() { return header_flag_; }
Request::Flag Request::getTransferFlag() { return transfer_flag_; }

// setter
void Request::setMethod(std::string method) {
  if (method == "GET")
    method_ = kGet;
  else if (method == "POST")
    method_ = kPost;
  else if (method == "DELETE")
    method_ = kDelete;
  else
    method_ = kNone;
}
void Request::setPath(std::string path) { path_ = path; }
void Request::setVersion(std::string version) { version_ = version; }
void Request::setHeader(std::string key, std::string value) {
  header_.setHeader(key, value, method_);
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
  size_t      pos = 0;
  std::string method;

  // parse method and switch to flag.
  method = ft::get_until_delimiter(start_line, " ", pos);
  if (method == "GET")
    method_ = kGet;  // 1
  else if (method == "POST")
    method_ = kPost;  // 2
  else if (method == "DELETE")
    method_ = kDelete;  // 3
  else
    method_ = kNone;  // 0

  path_ = ft::get_until_delimiter(start_line, " ", pos);
  version_ = start_line.erase(0, pos);
}

void Request::parse_header(std::string& request_message, size_t& pos) {
  std::string header_field;
  std::string field_name;

  // Iterate until the end of the header.
  while ((header_field =
              ft::get_until_delimiter(request_message, "\r\n", pos)) != "") {
    // split field name and value.
    field_name = ft::split_until_delimiter(header_field, ":");
    // trim spaces from value.
    header_field = ft::str_bidirectional_trim(header_field, " ");
    setHeader(field_name, header_field);
  }
}

void Request::parse_body(std::string& request_message, size_t& pos) {
  // Get a body trimmed CL/RF.
  size_t new_pos = request_message.find_first_not_of("\r\n", pos);
  body_ = request_message.erase(0, new_pos);
}

// print
void Request::print() {
  typedef std::map<std::string, std::string>::iterator iterator;
  // status code
  std::cout << "====== status code ========" << std::endl;
  std::cout << method_ << " " << path_ << " " << version_ << std::endl;
  // headers
  std::cout << "====== headers ========" << std::endl;
  const HeaderMap headerMap = header_.getHeaders();
  for (HeaderMap::const_iterator it = headerMap.begin(); it != headerMap.end();
       it++) {
    std::cout << it->first << ": " << it->second << std::endl;
  }
  // body
  std::cout << "====== body ========" << std::endl;
  std::cout << body_ << std::endl;
}
