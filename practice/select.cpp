#include <iostream>
#include <map>
#include <vector>

#include "Receiver.hpp"

void run(std::map<int, bool>& server_socket_map) {
  int&           fd_max = Receiver::get_fd_max();
  fd_set&        read_fds = Receiver::get_read_fds();
  fd_set         cp_read_fds;
  sockaddr_in    client_address;
  socklen_t      addr_size;
  struct timeval timeout;
  int            client_socket, str_len, fd_num, i;
  char           buf[BUF_SIZE];

  while (1) {
    std::cout << fd_max << std::endl;
    cp_read_fds = read_fds;

    timeout.tv_sec = 5;
    timeout.tv_usec = 5000;

    if ((fd_num = select(fd_max + 1, &cp_read_fds, 0, 0, &timeout)) == -1)
      break;
    if (fd_num == 0)  // means time limit is expired
      continue;
    for (i = 0; i < fd_max + 1; i++) {
      if (FD_ISSET(i, &cp_read_fds)) {
        if (server_socket_map[i]) {
          addr_size = sizeof(client_address);
          client_socket = accept(i, (struct sockaddr*)&client_address, &addr_size);
          FD_SET(client_socket, &read_fds);
          if (fd_max < client_socket)
            fd_max = client_socket;
          std::cout << "connected client: " << i << std::endl;
        } else {
          if ((str_len = read(i, buf, BUF_SIZE)) == 0) {
            FD_CLR(i, &read_fds);
            close(i);
            std::cout << "closed client: " << i << std::endl;
            break;
          } else {
            write(1, buf, str_len);
            write(i, buf, str_len);
          }
        }
      }
    }
  }
  close(i);
  return;
}

int main(int argc, char** argv) {
  int                   port;
  std::vector<Receiver> receivers;
  std::map<int, bool>   server_socket_map;

  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
    return 1;
  }

  FD_ZERO(&Receiver::get_read_fds());
  for (int i = 1; i < argc; i++) {
    port = atoi(argv[i]);
    receivers.push_back(Receiver(port));
    receivers[i - 1].bind_and_listen();
    server_socket_map[receivers[i - 1].get_server_socket()] = true;
    std::cout << "port: " << port << std::endl;
  }
  run(server_socket_map);

  return 0;
}