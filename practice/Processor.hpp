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

  int      status_code_;
  Request  request_;
  Response response_;

 public:
  Processor();
  ~Processor();

  void Process();

  int&      get_status_code();
  Request&  get_request();
  Response& get_response();

  void set_status_code(int status_code);
};

#endif