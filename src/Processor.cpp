#include "Processor.hpp"

#include <unistd.h>

#include <exception>
#include <iostream>

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
  if (response_.isBuilt() != false)
    return;
  ft::log.writeTimeLog("[Processor] --- Set config ---");
  ConfigServer config_server = getConfigServerForRequest(total_config, listen);
  findLocation(config_server);
  Config::printLocation(config_, "");
  // Config::printServer(config_server);
  file_manager_.setPath(config_.getRoot() + request_.getPath());
  try {
    (this->*method_func_map_[request_.getMethod()])();
    response_.build();
  } catch (...) {
    // response_.buildException();
  }
}

void Processor::findLocation(const ConfigServer& config) {
  size_t      last_slash_pos = request_.getPath().find_last_of("/");
  std::string tmp_location, location = "/";

  while (last_slash_pos != std::string::npos) {
    tmp_location = request_.getPath().substr(0, last_slash_pos);
    if (config.getLocation().find(tmp_location) != config.getLocation().end()) {
      location = tmp_location;
      break;
    }
    last_slash_pos = tmp_location.find_last_of("/", last_slash_pos - 1);
  }

  config_ = config.getLocation().at(location);
}

int Processor::parseRequest(MessageType request_message) {
  try {
    return request_.parse(request_message);
  } catch (RequestException& e) {
    ft::log.writeTimeLog("[Processor] --- Parsing request failed ---");
    ft::log.writeLog("Reason: " + std::string(e.what()));
    response_.buildException(e.getStatusCode());
    return -1;
  }
}

std::string Processor::strRequest() { return request_.printToString(); }

void Processor::methodGet() {
  // TODO: autoindex case

  // default case
  // FIXME: MIME 구현 후 수정
  // - set content type
  if (file_manager_.isExist()) {
    response_.setHeader("Content-Type", "text/html");
    // - set body
    response_.setBody(file_manager_.getContent());
    response_.setStatusCode(200);
  } else {
    response_.setStatusCode(404);
  }
}

void Processor::methodPost() {
  // set body(requested body)
  response_.setBody(request_.getBody());
  // FIXME: MIME 구현 후 수정
  // set content type
  response_.setHeader("Content-Type", "text/html");
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

void Processor::methodPut() {}

void Processor::methodDelete() {
  if (file_manager_.isExist()) {
    // if file is exist, delete file. 200
    // 204 means no content
    file_manager_.remove();
    response_.setStatusCode(204);
  } else {
    // if file isn't exist, 404
    response_.setStatusCode(404);
  }
}

void Processor::methodHead() {
  if (file_manager_.isExist()) {
    // - set body
    response_.setStatusCode(200);
  } else {
    response_.setStatusCode(404);
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
