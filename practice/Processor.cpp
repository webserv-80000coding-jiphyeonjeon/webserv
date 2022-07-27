#include "Processor.hpp"

std::map<int, std::string> Processor::createStatusCodes() {
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

const std::map<int, std::string> Processor::kStatusCodes = createStatusCodes();

Processor::Processor() : status_code_(200), request_(), response_() {}
Processor::~Processor() {}

void Processor::parseRequest(std::string request_message) {
  request_.parse(request_message);
}
void Processor::printRequest() { request_.print(); }

const int&      Processor::getStatusCode() { return status_code_; }
const Request&  Processor::getRequest() { return request_; }
const Response& Processor::getResponse() { return response_; }