#ifndef WEBSERV_PRACTICE_REQUEST_HPP_
#define WEBSERV_PRACTICE_REQUEST_HPP_

#include <map>
#include <string>

#include "RequestHeader.hpp"

class Request {
 public:
  typedef int                      Flag;
  typedef RequestHeader::HeaderMap HeaderMap;

  Request();
  ~Request();

  Flag          getMethod();
  std::string   getPath();
  std::string   getVersion();
  std::string   getHeader(std::string key);
  std::string   getBody();
  RequestHeader getHeader();
  Flag          getState();
  Flag          getHeaderFlag();
  Flag          getTransferFlag();

  void setMethod(std::string method);
  void setPath(std::string path);
  void setVersion(std::string version);
  void setHeader(std::string key, std::string value);
  void setBody(std::string body);

  void concatenate_request(std::string request_message);
  void parse(std::string request_message);
  void print();

 private:
  void parse_start_line(std::string start_line);
  void parse_header(std::string& header, size_t& pos);
  void parse_body(std::string& body_line, size_t& pos);

  std::string   request_message_;
  Flag          method_;
  std::string   path_;
  std::string   version_;
  std::string   body_;
  RequestHeader header_;
  Flag          state_;
  Flag          header_flag_;
  Flag          transfer_flag_;
};

#endif