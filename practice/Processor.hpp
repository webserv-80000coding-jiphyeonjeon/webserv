#ifndef WEBSERV_PRACTICE_PROCESSOR_HPP_
#define WEBSERV_PRACTICE_PROCESSOR_HPP_

#include <map>
#include <string>
#include <vector>

#include "Request.hpp"
#include "Response.hpp"

class Processor {
 public:
  typedef void (Processor::*Method)();
  typedef std::map<Request::Flag, Method> MethodMap;

  Processor();
  ~Processor();

  void parseRequest(std::string request_message);
  void printRequest();
  void printResponse();

  void process();

  const int&      getStatusCode();
  const Request&  getRequest();
  const Response& getResponse();

  void set_status_code(int status_code);

  void methodGet();
  void methodPost();
  void methodDelete();

 private:
  MethodMap methodMap_;

  MethodMap createMethodMap();

  int      fd_;
  int      status_code_;
  Request  request_;
  Response response_;
};

#endif