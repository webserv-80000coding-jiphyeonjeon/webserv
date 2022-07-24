#ifndef WEBSERV_PRACTICE_PROCESSOR_HPP_
#define WEBSERV_PRACTICE_PROCESSOR_HPP_

#include <map>
#include <string>
#include <vector>

#include "Request.hpp"
#include "Response.hpp"

class Processor {
 private:
  static const std::map<int, std::string> kStatusCodes;

  static std::map<int, std::string> createStatusCodes();

  int      status_code;
  Request  request;
  Response response;

 public:
  Processor();

  void Process();
};

#endif