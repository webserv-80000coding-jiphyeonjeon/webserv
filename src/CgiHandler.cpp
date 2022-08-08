#include "CgiHandler.hpp"

#include <fcntl.h>
#include <unistd.h>

#include <cstdlib>

#include "FileManager.hpp"
#include "Log.hpp"
#include "Utilities.hpp"
#include "Webserv.hpp"

extern char** environ;

CgiHandler::CgiHandler(const CgiPathType& cgi_path, const Request& request)
    : cgi_path_(cgi_path), body_(request.getBody()) {
  if (initEnviron(request) == -1)
    throw 500;
}

CgiHandler::~CgiHandler() {}

const CgiHandler::StatusCodeType& CgiHandler::getStatusCode() const {
  return status_code_;
}
const CgiHandler::HeaderMapType& CgiHandler::getHeaderMap() const {
  return header_map_;
}
const CgiHandler::BodyType& CgiHandler::getBody() const { return body_; }

void CgiHandler::cgiExecute() {
  if (initPipe() == -1)
    throw 500;

  FileManager file_manager("./www/tmp");

  pid_t pid = fork();
  if (pid == -1) {
    throw 500;
  } else if (!pid) {
    ft::log.writeLog("[Cgi] --- Child Process ---");
    int dup_error = 0;
    int tmp_fd;
    file_manager.createFile("");
    tmp_fd = open(file_manager.getPath().c_str(), O_WRONLY);

    close(fpipe_[1]);
    close(bpipe_[0]);

    dup_error |= dup2(fpipe_[0], STDIN_FILENO);
    dup_error |= dup2(tmp_fd, STDOUT_FILENO);

    close(fpipe_[0]);
    close(bpipe_[1]);

    if (dup_error == -1)
      throw 500;

    char* argv[2] = {strdup(cgi_path_.c_str()), NULL};
    execve(cgi_path_.c_str(), argv, environ);
    exit(EXIT_FAILURE);

  } else {
    ft::log.writeLog("[Cgi] --- Parent Process ---");
    close(fpipe_[0]);
    // close(bpipe_[1]);

    size_t offset = 0;

    while (1) {
      fd_set         write_fds;
      struct timeval timeout = {1, 0};
      int            state = Webserv::kTimeOut;

      while (state == Webserv::kTimeOut) {
        FD_ZERO(&write_fds);
        FD_SET(fpipe_[1], &write_fds);
        state = select(fpipe_[1] + 1, NULL, &write_fds, NULL, &timeout);
      }

      if (state > Webserv::kTimeOut) {
        if (FD_ISSET(fpipe_[1], &write_fds)) {
          ssize_t send_size =
              write(fpipe_[1], body_.c_str() + offset, body_.size() - offset);

          if (send_size == -1) {
            close(fpipe_[1]);
            throw 500;
          }

          offset += send_size;
          if (offset == body_.size()) {
            offset = 0;
            close(fpipe_[1]);
            break;
          }
        }
      } else {
        throw 500;
      }
    }

    waitpid(-1, NULL, 0);

    ft::log.writeLog("[Cgi] --- Child Process Finished ---");

    std::string cgi_response = file_manager.getContent();
    file_manager.remove();
    parseCgiResponse(cgi_response);
    // ft::log.writeTimeLog("[CGI] --- New body ---");
    // ft::log.writeLog(body_);
  }
}

void CgiHandler::parseCgiResponse(CgiResponseType& cgi_response) {
  size_t          pos = 0;
  HeaderKeyType   key;
  HeaderValueType value;
  std::string   status_line = ft::getUntilDelimiter(cgi_response, "\r\n", pos);
  std::string   header_field;
  HeaderMapType header_map;

  status_code_ =
      atoi(status_line.substr(status_line.find_first_of("12345"), 3).c_str());

  while (cgi_response.find("\r\n", pos) != std::string::npos) {
    std::string header = ft::getUntilDelimiter(cgi_response, "\r\n", pos);

    // End of header.
    if (header == "")
      break;

    key = ft::splitUntilDelimiter(header, ": ");
    value = ft::strBidirectionalTrim(header, " ");
    header_map[key] = value;
    // std::cout << "key: " << key << " value: " << value << std::endl;
  }

  body_ = cgi_response.erase(0, pos);
  // std::cout << body_.size() << std::endl;
}

int CgiHandler::initEnviron(const Request& request) {
  const char* method_list[] = {"GET", "POST", "PUT", "DELETE", "HEAD"};
  bool        secret_header = false;

  if (request.getHeaderMap().find("X-Secret-Header-For-Test") !=
      request.getHeaderMap().end()) {
    secret_header = true;
  }

  if (setenv("SERVER_PROTOCOL", "HTTP/1.1", 1) ||
      setenv("REQUEST_METHOD", method_list[request.getMethod() - 1], 1) ||
      setenv("PATH_INFO", cgi_path_.c_str(), 1) ||
      (secret_header &&
       setenv("HTTP_X_SECRET_HEADER_FOR_TEST",
              request.getHeader("X-Secret-Header-For-Test").c_str(), 1)))
    return -1;
  return 0;
}

int CgiHandler::initPipe() {
  if (pipe(fpipe_) || pipe(bpipe_))
    return -1;
  if (fcntl(fpipe_[1], F_SETFL, O_NONBLOCK) == -1 ||
      fcntl(bpipe_[0], F_SETFL, O_NONBLOCK) == -1)
    return -1;
  return 0;
}
