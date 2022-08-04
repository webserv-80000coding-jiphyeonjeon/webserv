#include "Processor.hpp"

#include <exception>
#include <iostream>

#include "Config.hpp"
#include "Log.hpp"

Processor::Processor() : status_code_(200) { initMethodFuncMap(); }

Processor::~Processor() {}

const File& Processor::getFile() const { return file_; }

const Processor::StatusCodeType& Processor::getStatusCode() const {
  return status_code_;
}

const Request& Processor::getRequest() const { return request_; }

const Level& Processor::getLevel() const { return request_.getLevel(); }

void Processor::setConfig(const ConfigServer& config) { config_ = config; }

void Processor::setFile(const File& file) { file_ = file; }

void Processor::setStatusCode(const StatusCodeType& status_code) {
  status_code_ = status_code;
}

void Processor::setRequest(const Request& request) { request_ = request; }

void Processor::process(const Config&                   total_config,
                        const ConfigServer::ListenType& listen) {
  ft::log.writeTimeLog("[Processor] --- Set config ---");
  config_ = getConfigServerForRequest(total_config, listen);
  Config::printServer(config_);
  // (this->*method_func_map_[request_.getMethod()])();
}

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
