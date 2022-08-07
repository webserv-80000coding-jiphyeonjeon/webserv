#include "CgiHandler.hpp"

#include <fcntl.h>
#include <unistd.h>

#include <cstdlib>

#include "Log.hpp"

extern char** environ;

CgiHandler::CgiHandler(const CgiPathType& cgi_path, const Request& request)
    : cgi_path_(cgi_path), body_(request.getBody()) {
  if (initEnviron(request) == -1)
    throw 500;
}

CgiHandler::~CgiHandler() {}

const CgiHandler::BodyType& CgiHandler::getBody() const { return body_; }

void CgiHandler::cgiExecute() {
  if (initPipe() == -1)
    throw 500;

  int    send_size = 1;
  size_t offset = 0;
  while (send_size > 0) {
    send_size = write(fpipe_[1], body_.c_str() + offset, body_.size() - offset);
    offset += send_size;
    if (offset == body_.size())
      break;
  }
  ft::log.getLogStream() << "send_size: " << offset << std::endl;

  pid_t pid = fork();
  if (pid == -1) {
    throw 500;
  } else if (!pid) {
    ft::log.writeLog("[Cgi] --- Child Process ---");
    int dup_error = 0;

    close(fpipe_[1]);
    close(bpipe_[0]);

    dup_error |= dup2(fpipe_[0], STDIN_FILENO);
    dup_error |= dup2(bpipe_[1], STDOUT_FILENO);

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
    close(bpipe_[1]);

    // int    send_size = 1;
    // size_t offset = 0;
    // while (send_size > 0) {
    //   send_size =
    //       write(fpipe_[1], body_.c_str() + offset, body_.size() - offset);
    //   offset += send_size;
    //   if (offset == body_.size())
    //     break;
    // }
    // ft::log.getLogStream() << "send_size: " << offset << std::endl;

    waitpid(-1, NULL, 0);
    ft::log.writeLog("[Cgi] --- Child Process Finished ---");

    BodyType new_body;
    char     buffer[kCgiBufferSize];
    int      read_size = 1;

    while (read_size > 0) {
      memset(buffer, 0, kCgiBufferSize);
      read_size = read(bpipe_[0], buffer, kCgiBufferSize - 1);
      new_body += buffer;
    }
    body_ = new_body;
    ft::log.writeTimeLog("[CGI] --- New body ---");
    ft::log.writeLog(body_);
  }
  close(fpipe_[1]);
  close(bpipe_[0]);

  // 여기서 파싱이 진행될 듯
}

int CgiHandler::initEnviron(const Request& request) {
  const char* method_list[] = {"GET", "POST", "PUT", "DELETE", "HEAD"};

  if (setenv("SERVER_PROTOCOL", "HTTP/1.1", 1) ||
      setenv("REQUEST_METHOD", method_list[request.getMethod() - 1], 1) ||
      setenv("PATH_INFO", cgi_path_.c_str(), 1))
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
