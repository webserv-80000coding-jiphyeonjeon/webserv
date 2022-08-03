#ifndef WEBSERV_PROCESSOR_HPP
#define WEBSERV_PROCESSOR_HPP

#include <map>
#include <string>
#include <vector>

#include "Request.hpp"

class Processor {
 public:
  typedef void (Processor::*MethodFuncType)();
  typedef std::map<Method, MethodFuncType> MethodFuncMapType;

  typedef Request::MessageType      MessageType;
  typedef Request::StatusCodeType   StatusCodeType;
  typedef Request::RequestException RequestException;
  // TODO 현재는 Fd로 관리하지만, 추후에 File 클래스로 관리할 예정
  typedef int FdType;

  Processor();
  ~Processor();

  const FdType&         getFd() const;
  const StatusCodeType& getStatusCode() const;
  const Request&        getRequest() const;
  // const Response& getResponse() const;

  void setFd(const FdType& fd);
  void setStatusCode(const StatusCodeType& status_code);
  void setRequest(const Request& request);

  void process();

  int  parseRequest(MessageType request_message);
  void printRequest();
  void printResponse();

  void methodGet();
  void methodPost();
  void methodPut();
  void methodDelete();
  void methodHead();

 private:
  MethodFuncMapType initMethodFuncMap();
  MethodFuncMapType method_func_map_;

  FdType         fd_;
  StatusCodeType status_code_;
  Request        request_;
  // Response       response_;
};

#endif