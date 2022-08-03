#include "Processor.hpp"

#include <exception>
#include <iostream>

#include "Log.hpp"

Processor::Processor() { initMethodFuncMap(); }

Processor::~Processor() {}

const Processor::FdType& Processor::getFd() const { return fd_; }

const Processor::StatusCodeType& Processor::getStatusCode() const {
  return status_code_;
}

const Request& Processor::getRequest() const { return request_; }

const Level& Processor::getLevel() const { return request_.getLevel(); }

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
    ft::log.writeTimeLog("[Processor] --- Parsing request failed ---");
    ft::log.writeLog("Reason: " + std::string(e.what()));
    // TODO 응답 메세지 작성 및 응답 준비
    // readyToResponse(e.getStatusCode());
    return -1;
  }
}

// void Processor::printRequest() { request_.print(); }

std::string Processor::strRequest() { return request_.printToString(); }

void Processor::printResponse() {
  // response_.print();
}

void Processor::methodGet() {}

void Processor::methodPost() {}

void Processor::methodPut() {}

void Processor::methodDelete() {}

void Processor::methodHead() {}

void Processor::initMethodFuncMap() {
  method_func_map_[kGet] = &Processor::methodGet;
  method_func_map_[kPost] = &Processor::methodPost;
  method_func_map_[kPut] = &Processor::methodPut;
  method_func_map_[kDelete] = &Processor::methodDelete;
  method_func_map_[kHead] = &Processor::methodHead;
}
