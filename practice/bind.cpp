#include <netinet/in.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <cstring>
#include <iostream>

int main() {
  struct sockaddr_in addr;
  struct sockaddr_in bf_addr;
  struct sockaddr_in af_addr;
  bzero(&addr, sizeof(addr));

  addr.sin_family = AF_INET;
  addr.sin_port = htons(4242);
  addr.sin_addr.s_addr = htonl(INADDR_ANY);

  int sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
    printf("bind() error\n");
    exit(1);
  }

  socklen_t bf_len = sizeof(bf_addr);
  getsockname(sockfd, (struct sockaddr*)&bf_addr, &bf_len);
  std::cout << "bf host : " << ntohs(bf_addr.sin_port) << std::endl;

  bzero(&addr, sizeof(addr));

  socklen_t af_len = sizeof(af_addr);
  getsockname(sockfd, (struct sockaddr*)&af_addr, &af_len);
  std::cout << "af host : " << ntohs(af_addr.sin_port) << std::endl;
}