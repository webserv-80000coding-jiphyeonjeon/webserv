#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "color.h"

#define BUFFER_SIZE 1024

void error_handling(char* message) {
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}

int main(int argc, char* argv[]) {
  int  server_socket;
  int  client_socket;
  char message[BUFFER_SIZE];
  int  str_len;

  if (argc != 2)
    error_handling(RED "Usage: ./server <port>" END);

  struct sockaddr_in server_addr;
  struct sockaddr_in client_addr;
  socklen_t          client_addr_size;

  struct timeval timeout;
  fd_set         reads, cpy_reads;
  int            fd_max, fd_num, i;

  server_socket = socket(PF_INET, SOCK_STREAM, 0);
  if (server_socket == -1)
    error_handling(RED "socket() error" END);

  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  server_addr.sin_port = htons(atoi(argv[1]));

  if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
    error_handling(RED "bind() error" END);

  if (listen(server_socket, 5) == -1)
    error_handling(RED "listen() error" END);

  FD_ZERO(&reads);
  FD_SET(server_socket, &reads);
  fd_max = server_socket;

  while (1) {
    cpy_reads = reads;
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;

    if ((fd_num = select(fd_max + 1, &cpy_reads, 0, 0, &timeout)) == -1)
      break;

    if (fd_num == 0)
      continue;

    for (i = 0; i <= fd_max; i++) {
      if (FD_ISSET(i, &cpy_reads)) {
        if (i == server_socket) {
          client_addr_size = sizeof(client_addr);
          client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_size);
          if (client_socket == -1)
            error_handling("accept() error");
          FD_SET(client_socket, &reads);
          if (fd_max < client_socket)
            fd_max = client_socket;
          printf(GRN "Accepted client: %d" END "\n", client_socket);
        } else {
          str_len = read(i, message, BUFFER_SIZE);
          if (str_len == 0) {
            FD_CLR(i, &reads);
            close(i);
            printf(RED "Client %d is closed" END "\n", i);
          } else {
            write(i, message, str_len);
            write(1, message, str_len);
          }
        }
      }
    }
  }

  close(server_socket);
  return 0;
}
