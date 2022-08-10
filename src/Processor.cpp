#include "Processor.hpp"

#include <unistd.h>

#include <exception>
#include <iostream>
#include <string>
#include <vector>

#include "CgiHandler.hpp"
#include "Config.hpp"
#include "Log.hpp"

Processor::Processor() : offset_(0), io_count_(0), status_code_(200) {
  initMethodFuncMap();
}

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

const Processor::OffsetType& Processor::getOffset() const { return offset_; }

const Processor::OffsetType& Processor::getIoCount() const { return io_count_; }

void Processor::setStatusCode(const StatusCodeType& status_code) {
  status_code_ = status_code;
}
void Processor::setRequest(const Request& request) { request_ = request; }

void Processor::setOffset(const OffsetType& offset) { offset_ = offset; }

void Processor::setIoCount(const OffsetType& io_count) { io_count_ = io_count; }

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
    if (request_.getBody().size() > config_.getClientBodyBufferSize())
      throw ProcessException("Body size over limit", 413);

    (this->*method_func_map_[request_.getMethod()])();
    response_.build();

  } catch (const ProcessException& e) {
    ft::log.writeTimeLog("[Processor] --- Process failed ---");
    ft::log.writeLog("Reason: " + std::string(e.what()) + " " +
                     ft::toString(e.getStatusCode()));
    // if there's error page, use it
    if (config_.getErrorPage().find(e.getStatusCode()) !=
        config_.getErrorPage().end()) {
      file_manager_.setPath(config_.getRoot() +
                            config_.getErrorPage().at(e.getStatusCode()));
      response_.setBody(file_manager_.getContent());
      response_.setStatusCode(e.getStatusCode());
      response_.build();
    } else {
      response_.buildException(e.getStatusCode());
    }
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
  // Config::printLocation(config_, "");

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

bool Processor::isRequestExpired() { return request_.isExpired(); }

void Processor::methodGet() {
  if (config_.getReturn().first) {
    response_.setStatusCode(config_.getReturn().first);
    response_.setHeader("Location", config_.getReturn().second);
    return;
  }
  // 폴더
  if (file_manager_.isDirectory()) {
    if (access(file_manager_.getPath().c_str(), R_OK) != 0)
      throw ProcessException("Forbidden", 403);

    if (config_.getAutoindex()) {
      // autoindex: on -> 바로 autoindex page 보내주기 (200)
      response_.setStatusCode(200);
      response_.setBody(generateDirList());
      response_.setHeader("Content-Type", "text/html");
      return;
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
    if (access(file_manager_.getPath().c_str(), R_OK) != 0)
      throw ProcessException("Forbidden", 403);
    response_.setHeader("Content-Type", ft::getMIME(file_manager_));
    response_.setBody(file_manager_.getContent());
    response_.setStatusCode(200);
  } else if (!file_manager_.isExist()) {
    throw ProcessException("File not found", 404);
  }
}

void Processor::methodPost() {
  const ConfigLocation::CgiType&          cgi_list = config_.getCgi();
  ConfigLocation::CgiType::const_iterator it;
  if ((it = cgi_list.find(file_manager_.getExtension())) != cgi_list.end()) {
    ft::log.writeTimeLog("[Processor] --- Execute cgi ---");

    if (!FileManager::isExist(it->second) ||
        FileManager::isDirectory(it->second))
      throw ProcessException("CGI program not found", 404);

    if (access(it->second.c_str(), X_OK) != 0)
      throw ProcessException("Forbidden", 403);

    try {
      CgiHandler cgi(it->second, request_);
      cgi.cgiExecute();
      response_.setStatusCode(cgi.getStatusCode());
      response_.setHeaderMap(cgi.getHeaderMap());
      response_.setBody(cgi.getBody());
    } catch (const int& e) {
      throw ProcessException("CGI execution failed", e);
    }
    return;
  }
  // set body(requested body)
  response_.setBody(request_.getBody());
  response_.setHeader("Content-Type", ft::getMIME(file_manager_));
  if (file_manager_.isExist() == false) {
    prepareBeforeCreate();
    // if file isn't exist, create file. 201
    file_manager_.createFile(request_.getBody());
    response_.setStatusCode(201);
  } else {
    if (file_manager_.isDirectory())
      throw ProcessException("Is directory", 409);
    // if file is exist, update file. 200
    file_manager_.appendContent(request_.getBody());
    response_.setStatusCode(200);
  }
}

void Processor::methodPut() {
  response_.setBody(request_.getBody());
  response_.setHeader("Content-Type", ft::getMIME(file_manager_));
  if (file_manager_.isExist() == false) {
    // std::cout << file_manager_.getPath() << std::endl;
    prepareBeforeCreate();
    // if file isn't exist, create file. 201
    file_manager_.createFile(request_.getBody());
    response_.setStatusCode(201);
  } else if (request_.getBody().empty()) {
    file_manager_.updateContent("");
    response_.setStatusCode(204);
  } else {
    if (file_manager_.isDirectory())
      throw ProcessException("Is directory", 409);
    // if file is exist, update file. 200
    file_manager_.updateContent(request_.getBody());
    response_.setStatusCode(200);
  }
}

void Processor::methodDelete() {
  if (file_manager_.isExist()) {
    // if file is exist, delete file. 200
    if (file_manager_.getContent().empty()) {
      response_.setStatusCode(204);
      file_manager_.remove();
    } else {
      response_.setBody(file_manager_.getContent());
      response_.setHeader("Content-Type", ft::getMIME(file_manager_));
      response_.setStatusCode(200);
      file_manager_.remove();
    }
  } else {
    // if file isn't exist, 404
    throw ProcessException("File not found", 404);
  }
}

void Processor::methodHead() {
  methodGet();
  response_.setBody("");
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

void Processor::prepareBeforeCreate() {
  // if directory is not exist until path, create directory.
  PathType path_until_last_dir = file_manager_.getPath().substr(
      0, file_manager_.getPath().find_last_of("/"));

  // last dir is exist
  if (FileManager::isDirectory(path_until_last_dir)) {
    if (access(path_until_last_dir.c_str(), W_OK != 0))
      throw ProcessException("No write permission to dir", 403);
    else
      return;
  }

  PathType path_for_check = path_until_last_dir.substr(
      0, path_until_last_dir.find_first_of("/", 1) - 1);

  // update path_for_check until is not exist.
  while (FileManager::isExist(path_for_check)) {
    // if path_for_check is not dir, throw 409(conflict)
    if (!FileManager::isDirectory(path_for_check))
      throw ProcessException("File is in the path", 409);
    // if no permission to create directory, throw exception.
    if (access(path_for_check.c_str(), W_OK != 0))
      throw ProcessException("No write permission to dir", 403);
    path_for_check = path_until_last_dir.substr(
        0, path_until_last_dir.find_first_of("/", path_for_check.size() + 2));
  }

  // create directory until last dir.
  while (path_for_check != path_until_last_dir) {
    path_for_check = path_until_last_dir.substr(
        0, path_until_last_dir.find_first_of("/", path_for_check.size() + 2));
    mkdir(path_for_check.c_str(), 0755);
  }
}

std::string Processor::generateDirList() {
  std::vector<PathType> dir_list = file_manager_.getDirList();
  std::stringstream     ss;
  ss << "<html>\n<body>\n<h1>Index of " << request_.getPath() << "</h1>\n";
  ss << "<ul>\n";
  for (std::vector<PathType>::iterator it = dir_list.begin();
       it != dir_list.end(); ++it) {
    ss << "<li>" << *it << "</li>\n";
  }
  ss << "</ul>\n";
  ss << "</body>\n</html>\n";
  return ss.str();
}

Processor::ProcessException::ProcessException(const std::string&    message,
                                              const StatusCodeType& status_code)
    : ServerException(message, status_code) {}
