#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

  client_addr_size = sizeof(client_addr);
  client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_size);
  if (client_socket == -1)
    error_handling("accept() error");
  printf(GRN "Success to connect client" END);
  printf("\n");

  while ((str_len = read(client_socket, message, BUFFER_SIZE)) != 0) {
    write(client_socket, message, str_len);
    write(1, message, str_len);
  }

  close(server_socket);
  close(client_socket);

  return 0;
}
