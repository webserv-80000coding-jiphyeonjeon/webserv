#include "Request.hpp"

#include <map>
#include <string>
#include <vector>

Request::Request() {}
Request::~Request() {}
std::string                        Request::getMethod() { return method_; }
std::string                        Request::getPath() { return path_; }
std::string                        Request::getVersion() { return version_; }
std::string                        Request::getHeader(std::string key) { return headers_[key]; }
std::string                        Request::getBody() { return body_; }
std::map<std::string, std::string> Request::getHeaders() { return headers_; }
Request::Flag                      Request::getState() { return state_; }
Request::Flag                      Request::getHeaderFlag() { return header_flag_; }
Request::Flag                      Request::getTransferFlag() { return transfer_flag_; }

void Request::setMethod(std::string method) { method_ = method; }
void Request::setPath(std::string path) { path_ = path; }
void Request::setVersion(std::string version) { version_ = version; }
void Request::setHeader(std::string key, std::string value) { headers_[key] = value; }
void Request::setBody(std::string body) { body_ = body; }

void Request::parse(std::string request_message) {}
void Request::print() {}

// void Request::parse(std::string request_message) {
//   request_message_ = request_message;
//   std::vector<std::string> request_message_vector = split(request_message, '\n');
//   std::vector<std::string> request_message_vector_first = split(request_message_vector[0], ' ');
//   method_ = request_message_vector_first[0];
//   path_ = request_message_vector_first[1];
//   version_ = request_message_vector_first[2];
//   for (int i = 1; i < request_message_vector.size(); i++) {
//     std::vector<std::string> request_message_vector_second = split(request_message_vector[i],
//     ':'); headers_[request_message_vector_second[0]] = request_message_vector_second[1];
//   }
//   state_ = kContinuous;
//   header_flag_ = kDefault;
//   transfer_flag_ = kDefault;
// }