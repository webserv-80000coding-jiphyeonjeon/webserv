#ifndef WEBSERV_PROCESSOR_HPP
#define WEBSERV_PROCESSOR_HPP

#include <map>
#include <string>
#include <vector>

#include "Config.hpp"
#include "Request.hpp"

class Processor {
 public:
  typedef void (Processor::*MethodFuncType)();
  typedef std::map<Method, MethodFuncType> MethodFuncMapType;

  typedef Config::ServersType      ServersType;
  typedef Config::ServerNamesType  ServerNamesType;
  typedef Config::ListenListType   ListenListType;
  typedef ConfigServer::ListenType ListenType;

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
  const Level&          getLevel() const;
  // const Response& getResponse() const;

  void setConfig(const ConfigServer& config);
  void setFd(const FdType& fd);
  void setStatusCode(const StatusCodeType& status_code);
  void setRequest(const Request& request);

  void process(const Config& total_config, const ListenType& listen);

  int         parseRequest(MessageType request_message);
  void        printResponse();
  std::string strRequest();
  // void        printRequest();

  void methodGet();
  void methodPost();
  void methodPut();
  void methodDelete();
  void methodHead();

 private:
  MethodFuncMapType method_func_map_;

  ConfigServer   config_;
  FdType         fd_;
  StatusCodeType status_code_;
  Request        request_;
  // Response       response_;

  void initMethodFuncMap();

  ConfigServer getConfigServerForRequest(
      const Config& total_config, const ConfigServer::ListenType& listen);
};

#endif
