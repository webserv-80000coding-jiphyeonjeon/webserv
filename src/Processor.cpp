#include "Processor.hpp"

#include <exception>
#include <iostream>

Processor::Processor() : method_func_map_(initMethodFuncMap()) {}

Processor::~Processor() {}

const Processor::FdType& Processor::getFd() const { return fd_; }

const Processor::StatusCodeType& Processor::getStatusCode() const {
  return status_code_;
}

const Request& Processor::getRequest() const { return request_; }

void Processor::setFd(const FdType& fd) { fd_ = fd; }

void Processor::setStatusCode(const StatusCodeType& status_code) {
  status_code_ = status_code;
}

void Processor::setRequest(const Request& request) { request_ = request; }

void Processor::process() { (this->*method_func_map_[request_.getMethod()])(); }

int Processor::parseRequest(MessageType request_message) {
  try {
    return request_.parse(request_message);
  } catch (RequestException& e) {
    std::cout << e.what() << std::endl;
    // TODO: 응답 메세지 작성 및 응답 준비
    // readyToResponse(e.getStatusCode());
    return -1;
  }
}

void Processor::printRequest() { request_.print(); }

void Processor::printResponse() {
  // response_.print();
}

void Processor::methodGet() {}

void Processor::methodPost() {}

void Processor::methodPut() {}

void Processor::methodDelete() {}

void Processor::methodHead() {}

Processor::MethodFuncMapType Processor::initMethodFuncMap() {
  MethodFuncMapType method_func_map;
  method_func_map[kGet] = &Processor::methodGet;
  method_func_map[kPost] = &Processor::methodPost;
  method_func_map[kPut] = &Processor::methodPut;
  method_func_map[kDelete] = &Processor::methodDelete;
  method_func_map[kHead] = &Processor::methodHead;
  return method_func_map;
}