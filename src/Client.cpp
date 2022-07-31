#include <unistd.h>

#include <iostream>

#include "Webserv.hpp"
#include "color.hpp"

#define BUFFER_SIZE 1024

int main(int argc, char* argv[]) {
  int                client_socket;
  struct sockaddr_in server_addr;
  char               message[BUFFER_SIZE];
  int                str_len;

  if (argc != 3) {
    std::cerr << RED "Usage: ./client <ip> <port>" END << std::endl;
    return 1;
  }

  client_socket = socket(PF_INET, SOCK_STREAM, 0);
  if (client_socket == -1) {
    std::cerr << RED "socket() error" END << std::endl;
    return 1;
  }

  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = inet_addr(argv[1]);
  server_addr.sin_port = htons(atoi(argv[2]));

  if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
    std::cerr << RED "connect() error" END << std::endl;
    return 1;
  }

  std::cout << GRN "Connected to server" END << std::endl;

  while (1) {
    std::cout << "> ";
    std::cin.getline(message, BUFFER_SIZE);

    if (strcmp(message, "exit") == 0)
      break;
    send(client_socket, message, strlen(message), 0);

    str_len = recv(client_socket, message, BUFFER_SIZE, 0);
    std::cout << "from server: " << message << std::endl;
  }
  close(client_socket);
}