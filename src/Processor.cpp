#include "Processor.hpp"

#include <unistd.h>

#include <exception>
#include <iostream>
#include <string>
#include <vector>

#include "Config.hpp"
#include "Log.hpp"

Processor::Processor() : status_code_(200) { initMethodFuncMap(); }

Processor::~Processor() {}

const FileManager& Processor::getFileManager() const { return file_manager_; }
const Processor::StatusCodeType& Processor::getStatusCode() const {
  return status_code_;
}
const Request& Processor::getRequest() const { return request_; }
const Processor::ResponseMessageType& Processor::getResponseMessage() const {
  return response_.getMessage();
}
const Level& Processor::getLevel() const { return request_.getLevel(); }

void Processor::setStatusCode(const StatusCodeType& status_code) {
  status_code_ = status_code;
}
void Processor::setRequest(const Request& request) { request_ = request; }

void Processor::process(const Config&                   total_config,
                        const ConfigServer::ListenType& listen) {
  if (response_.isBuilt())
    return;
  ft::log.writeTimeLog("[Processor] --- Set config ---");
  ConfigServer config_server = getConfigServerForRequest(total_config, listen);
  // Config::printServer(config_server);
  findLocation(config_server);

  try {
    // 사용가능한 함수인 지 확인(limit-accept)
    const std::string method_list[] = {"GET", "POST", "PUT", "DELETE", "HEAD"};
    if (config_.getLimitExcept().find(method_list[request_.getMethod() - 1]) ==
        config_.getLimitExcept().end()) {
      throw ProcessException("Method not allowed(limit_except) " +
                                 method_list[request_.getMethod() - 1],
                             405);
    }
    (this->*method_func_map_[request_.getMethod()])();
    response_.build();
  } catch (const ProcessException& e) {
    ft::log.writeTimeLog("[Processor] --- Process failed ---");
    ft::log.writeLog("Reason: " + std::string(e.what()));
    response_.buildException(e.getStatusCode());
  }
}

void Processor::findLocation(const ConfigServer& config) {
  const ConfigServer::LocationType& locations = config.getLocation();
  std::string                       path = request_.getPath();

  while (locations.find(path) == locations.end()) {
    size_t pos = path.find_last_of('/');
    if (pos != 0) {
      path = path.substr(0, pos);
    } else {
      path = "/";
      break;
    }
  }

  config_ = locations.at(path);
  ft::log.writeLog("Location: " + path);
  Config::printLocation(config_, "");

  std::string result_path = request_.getPath();
  result_path =
      config_.getRoot() +
      (path.size() > 1 ? result_path.erase(0, path.size()) : result_path);
  if (FileManager::isDirectory(result_path) && *result_path.rbegin() != '/') {
    result_path += '/';
  }
  file_manager_.setPath(result_path);
  ft::log.writeTimeLog("[Processor] --- Set path ---");
  ft::log.writeLog(file_manager_.getPath());
}

int Processor::parseRequest(MessageType request_message) {
  try {
    return request_.parse(request_message);
  } catch (RequestException& e) {
    ft::log.writeTimeLog("[Processor] --- Parsing request failed ---");
    ft::log.writeLog(request_.getRequestMessage() +
                     "\nReason: " + std::string(e.what()));
    response_.buildException(e.getStatusCode());
    return -1;
  }
}

std::string Processor::strRequest() { return request_.printToString(); }

void Processor::methodGet() {
  // FIXME: autoindex
  // 폴더
  if (file_manager_.isDirectory()) {
    if (config_.getAutoindex()) {
      // autoindex: on -> 바로 autoindex page 보내주기 (200)
      // TODO: autoindex page 보내주기
    } else {
      // autoindex: off
      // getIndex()로 가져온 vector들에 해당하는 파일 확인하기 (200 / 404)
      IndexType index_list = config_.getIndex();

      for (IndexType::iterator it = index_list.begin(); it != index_list.end();
           ++it) {
        PathType path = file_manager_.getPath() + *it;
        if (file_manager_.isExist(path)) {
          file_manager_.setPath(path);
          break;
          // 파일로 가서 보내주기 (200)
        }
      }
      if (file_manager_.isDirectory()) {
        throw ProcessException("Directory index not found", 404);
      }
    }
  }

  // 파일
  // 해당 위치에 존재하는지만 판단 - (200 / 404)
  if (file_manager_.isExist() && !file_manager_.isDirectory()) {
    response_.setHeader("Content-Type",
                        ft::getMIME(file_manager_.getExtension()));
    response_.setBody(file_manager_.getContent());
    response_.setStatusCode(200);
  } else if (!file_manager_.isExist()) {
    throw ProcessException("File not found", 404);
  }
}

void Processor::methodPost() {
  if (config_.getCgi().find(file_manager_.getExtension()) !=
      config_.getCgi().end()) {
    // cgi 동작
  }
  // set body(requested body)
  response_.setBody(request_.getBody());
  response_.setHeader("Content-Type",
                      ft::getMIME(file_manager_.getExtension()));
  if (file_manager_.isExist() == false) {
    // if file isn't exist, create file. 201
    file_manager_.createFile(request_.getBody());
    response_.setStatusCode(201);
  } else {
    // if file is exist, update file. 200
    file_manager_.appendContent(request_.getBody());
    response_.setStatusCode(200);
  }
}

void Processor::methodPut() {
  if (file_manager_.isExist() == false) {
    std::cout << file_manager_.getPath() << std::endl;
    // if file isn't exist, create file. 201
    file_manager_.createFile(request_.getBody());
    response_.setStatusCode(201);
  } else if (request_.getBody().empty()) {
    file_manager_.updateContent("");
    response_.setStatusCode(204);
  } else {
    // if file is exist, update file. 200
    file_manager_.updateContent(request_.getBody());
    response_.setStatusCode(200);
  }
  response_.setBody(request_.getBody());
  response_.setHeader("Content-Type",
                      ft::getMIME(file_manager_.getExtension()));
}

void Processor::methodDelete() {
  if (file_manager_.isExist()) {
    // if file is exist, delete file. 200
    if (file_manager_.getContent().empty()) {
      response_.setStatusCode(204);
      file_manager_.remove();
    } else {
      response_.setBody(file_manager_.getContent());
      response_.setHeader("Content-Type",
                          ft::getMIME(file_manager_.getExtension()));
      response_.setStatusCode(200);
      file_manager_.remove();
    }
  } else {
    // if file isn't exist, 404
    throw ProcessException("File not found", 404);
  }
}

void Processor::methodHead() {
  if (file_manager_.isExist()) {
    // - set body
    // FIXME: MIME 구현 후 수정
    response_.setHeader("Content-Length",
                        ft::toString(file_manager_.getContent().size()));
    response_.setStatusCode(200);
  } else {
    throw ProcessException("File not found", 404);
  }
}

void Processor::initMethodFuncMap() {
  method_func_map_[kGet] = &Processor::methodGet;
  method_func_map_[kPost] = &Processor::methodPost;
  method_func_map_[kPut] = &Processor::methodPut;
  method_func_map_[kDelete] = &Processor::methodDelete;
  method_func_map_[kHead] = &Processor::methodHead;
}

ConfigServer Processor::getConfigServerForRequest(
    const Config& total_config, const Processor::ListenType& listen) {
  // 연결된 listen 기준으로 후보군 찾기
  // 연결이 되어있다는 뜻은 후보가 최소 한 개 이상은 반드시 있다는 뜻
  ServersType candidate_configs;

  const ServersType servers = total_config.getServers();
  for (ServersType::const_iterator candidate = servers.begin();
       candidate != servers.end(); ++candidate) {
    const ListenListType listen_list = candidate->getListen();
    for (ListenListType::const_iterator it_listen = listen_list.begin();
         it_listen != listen_list.end(); ++it_listen) {
      if (*it_listen == listen)
        candidate_configs.push_back(*candidate);
    }
  }

  // server_name 기준으로 찾고, 없으면 첫 번째 반환
  for (ServersType::const_iterator config = candidate_configs.begin();
       config != candidate_configs.end(); ++config) {
    const ServerNamesType server_name = config->getServerName();
    for (ServerNamesType::const_iterator it_server_name = server_name.begin();
         it_server_name != server_name.end(); ++it_server_name) {
      if (*it_server_name == request_.getServerName())
        return *config;
    }
  }
  return candidate_configs[0];
}

Processor::ProcessException::ProcessException(const std::string&    message,
                                              const StatusCodeType& status_code)
    : ServerException(message, status_code) {}
