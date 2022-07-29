#include "Processor.hpp"

#include <fcntl.h>
#include <unistd.h>

#include <iostream>

#include "Utility.hpp"

Processor::Processor()
    : status_code_(200),
      request_(),
      response_(),
      methodMap_(createMethodMap()) {}
Processor::~Processor() {}

void Processor::parseRequest(std::string request_message) {
  request_.parse(request_message);
}

void Processor::printRequest() { request_.print(); }
void Processor::printResponse() { response_.print(); }

void Processor::process() { (this->*methodMap_[request_.getMethod()])(); }

const int&      Processor::getStatusCode() { return status_code_; }
const Request&  Processor::getRequest() { return request_; }
const Response& Processor::getResponse() { return response_; }

void Processor::methodGet() {
  ssize_t     read_size = 0;
  std::string content;
  char        buffer[1024 + 1];

  fd_ = ::open("./index.html", O_RDONLY);
  while (true) {
    read_size = read(fd_, buffer, 1024);
    if (!read_size) {
      break;
    } else if (read_size == -1) {
      content = "";
      break;
    } else {
      buffer[read_size] = '\0';
      content.append(buffer, read_size);
    }
  }
  response_.setStatusCode(200);
  response_.setHeader("Content-Type", "text/html");
  response_.setHeader("Content-Length", ft::toString(content.size()));
  response_.setBody(content);
  response_.build();
}

void Processor::methodPost() {}

void Processor::methodDelete() {}

Processor::MethodMap Processor::createMethodMap() {
  MethodMap method_map;
  method_map[kGet] = &Processor::methodGet;
  method_map[kPost] = &Processor::methodPost;
  method_map[kDelete] = &Processor::methodDelete;

  return method_map;
}