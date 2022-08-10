#include "Webserv.hpp"

#include <iostream>

#include "Log.hpp"
#include "color.hpp"
#include "sys/time.h"

Webserv::Webserv(const Config& config)
    : config_(config),
      max_fd_(0),
      server_map_(),
      connect_socket_(),
      ready_to_write_() {
  FD_ZERO(&fd_set_);
}

Webserv::~Webserv() {}

void Webserv::initWebserv() {
  Config::ListenListType listen_list = config_.getAllListenList();

  for (Config::ListenListType::const_iterator listen = listen_list.begin();
       listen != listen_list.end(); ++listen) {
    Server server(*listen);
    FdType fd;

    server.initServer();
    fd = server.getFd();
    FD_SET(fd, &fd_set_);
    server_map_.insert(std::make_pair(fd, server));
    if (fd > max_fd_)
      max_fd_ = fd;

    std::cout << "📮 Socket " BWHT << listen->second << END " ready"
              << std::endl;
  }
  if (!max_fd_)
    throw std::runtime_error("Server: No listening.");
}

void Webserv::runWebserv() {
  while (1) {
    fd_set         read_fds;
    fd_set         write_fds;
    struct timeval timeout = {1, 0};  // tv_sec = 1, tv_usec = 0
    int            state = kTimeOut;

    std::string loading_dot[3] = {".  ", ".. ", "..."};
    int         loading = 0;

    // select 대기
    while (state == kTimeOut) {
      checkExpiredConnection();
      memcpy(&read_fds, &fd_set_, sizeof(fd_set_));
      FD_ZERO(&write_fds);
      for (ReadyType::iterator it = ready_to_write_.begin();
           it != ready_to_write_.end(); ++it)
        FD_SET(*it, &write_fds);

      std::cout << "\rWaiting" << loading_dot[loading++] << std::flush;
      if (loading == 3)
        loading = 0;

      state = select(max_fd_ + 1, &read_fds, &write_fds, NULL, &timeout);
    }

    // 변화에 따라 write -> read -> connection 순서로 처리
    if (state > kTimeOut) {
      addConnection(state, read_fds);
      receiveRequest(state, read_fds);
      sendResponse(state, write_fds);
    } else {
      selectError();
    }
    loading = 0;

    // TODO max_fd_ reset
    // server 소켓의 경우 select를 돌고 있다면 무조건 존재하는 것
    // map의 key는 fd라서 마지막 값이 곧 최대값
    FdServerMapType::const_iterator max_server = server_map_.end();
    max_fd_ = (--max_server)->first;

    // client 소켓은 없을 수도 있으니 사이즈 체크하고 최대값 접근
    ConnectSocketType::const_iterator max_client = connect_socket_.end();
    if (!connect_socket_.empty() && max_fd_ < (--max_client)->first)
      max_fd_ = max_client->first;
  }
}

void Webserv::addConnection(int& state, fd_set& read_fds) {
  for (FdServerMapType::iterator it = server_map_.begin();
       state && it != server_map_.end(); ++it) {
    if (FD_ISSET(it->first, &read_fds)) {
      FdType client_socket = it->second.acceptClient();
      if (client_socket != -1) {
        FD_SET(client_socket, &fd_set_);
        connect_socket_.insert(std::make_pair(client_socket, &it->second));
        if (client_socket > max_fd_)
          max_fd_ = client_socket;
      }
      std::cout << "\r" BYEL "🔌 Accept " END << it->second.getPort() << END
                << std::endl;
      state = 0;
      break;
    }
  }
}

void Webserv::receiveRequest(int& state, fd_set& read_fds) {
  FdType receive_socket;

  for (ConnectSocketType::iterator it = connect_socket_.begin();
       state && it != connect_socket_.end(); ++it) {
    receive_socket = it->first;
    if (FD_ISSET(receive_socket, &read_fds)) {
      RecvState recv_state = it->second->receiveData(receive_socket);

      if (recv_state == kClosedClient || recv_state == kRecvError) {
        FD_CLR(receive_socket, &fd_set_);
        FD_CLR(receive_socket, &read_fds);
        connect_socket_.erase(receive_socket);

      } else if (recv_state == kRecvSuccess || recv_state == kParseError) {
        it->second->process(receive_socket, config_);
        ready_to_write_.push_back(receive_socket);
      }
      // kRecvContinuous: do nothing
      state = 0;
      break;
    }
  }
}

void Webserv::sendResponse(int& state, fd_set& write_fds) {
  for (ReadyType::iterator it = ready_to_write_.begin();
       state && it != ready_to_write_.end(); ++it) {
    if (FD_ISSET(*it, &write_fds)) {
      SendState send_state = connect_socket_[*it]->sendData(*it);

      if (send_state == kSendError) {
        FD_CLR(*it, &fd_set_);
        FD_CLR(*it, &write_fds);
        connect_socket_.erase(*it);
        ready_to_write_.erase(it);
      } else if (send_state == kSendSuccess) {
        ready_to_write_.erase(it);
      }
      state = 0;
      break;
    }
  }
}

void Webserv::selectError() {
  for (ConnectSocketType::iterator it = connect_socket_.begin();
       it != connect_socket_.end(); ++it) {
    it->second->closeClient(it->first);
  }
  connect_socket_.clear();
  ready_to_write_.clear();
  FD_ZERO(&fd_set_);
  for (FdServerMapType::iterator it = server_map_.begin();
       it != server_map_.end(); ++it)
    FD_SET(it->first, &fd_set_);
  std::cout << "\rServer: select() error " << std::endl;
}

void Webserv::checkExpiredConnection() {
  for (FdServerMapType::iterator it = server_map_.begin();
       it != server_map_.end(); ++it) {
    std::vector<FdType> expired_clients = it->second.checkExpiredConnection();

    for (std::vector<FdType>::iterator it = expired_clients.begin();
         it != expired_clients.end(); ++it) {
      FD_CLR(*it, &fd_set_);
      connect_socket_.erase(*it);
    }
  }
}
