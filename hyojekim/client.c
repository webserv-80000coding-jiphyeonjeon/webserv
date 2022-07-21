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
  int                client_socket;
  struct sockaddr_in server_addr;
  char               message[BUFFER_SIZE];
  int                str_len;

  if (argc != 3)
    error_handling(RED "Usage: ./client <ip> <port>" END);

  client_socket = socket(PF_INET, SOCK_STREAM, 0);
  if (client_socket == -1)
    error_handling(RED "socket() error" END);

  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = inet_addr(argv[1]);
  server_addr.sin_port = htons(atoi(argv[2]));

  if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
    error_handling(RED "connect() error" END);

  printf(GRN "Success to connect server\n" END);

  while (1) {
    fputs("Input message (q to quit): ", stdout);
    fgets(message, BUFFER_SIZE, stdin);

    if (!strcmp(message, "q\n"))
      break;
    write(client_socket, message, strlen(message));

    str_len = read(client_socket, message, BUFFER_SIZE - 1);
    message[str_len] = 0;
    printf("Message from server: %s\n", message);
  }

  close(client_socket);
  return 0;
}