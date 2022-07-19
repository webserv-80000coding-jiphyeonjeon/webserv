#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

void error_handling(char* message) {
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}

int main(int argc, char* argv[]) {
  int                client_socket;
  struct sockaddr_in server_addr;
  char               message[1024] = {
                    0x00,
  };

  client_socket = socket(PF_INET, SOCK_STREAM, 0);
  if (client_socket == -1) {
    error_handling("socket() error");
  }

  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = inet_addr(argv[1]);
  server_addr.sin_port = htons(atoi(argv[2]));

  if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
    error_handling("connect() error");
  }
  printf("Success to connect server\n");

  if (read(client_socket, message, sizeof(message)) == -1) {
    error_handling("read() error");
  }
  printf("%s", message);

  close(client_socket);
  return 0;
}