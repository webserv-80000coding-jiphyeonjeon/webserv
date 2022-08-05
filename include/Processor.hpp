#ifndef WEBSERV_PROCESSOR_HPP
#define WEBSERV_PROCESSOR_HPP

#include <map>
#include <string>
#include <vector>

#include "Config.hpp"
#include "File.hpp"
#include "Request.hpp"
#include "Response.hpp"

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
  typedef Request::PathType         PathType;

  typedef Response::MessageType ResponseMessageType;

  Processor();
  ~Processor();

  const File&           getFile() const;
  const StatusCodeType& getStatusCode() const;
  const Request&        getRequest() const;
  const Level&          getLevel() const;
  // const Response& getResponse() const;

  const ResponseMessageType& getResponseMessage() const;

  void setConfig(const ConfigServer& config);
  void setFile(const File& file);
  void setStatusCode(const StatusCodeType& status_code);
  void setRequest(const Request& request);

  void process(const Config& total_config, const ListenType& listen);

  void findLocation(const ConfigServer& config);

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
  File           file_;
  StatusCodeType status_code_;
  Request        request_;
  Response       response_;

  void initMethodFuncMap();

  ConfigServer getConfigServerForRequest(
      const Config& total_config, const ConfigServer::ListenType& listen);
};

#endif
