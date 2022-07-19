#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

#define BUF_SIZE 100

void error_handling(char *buf);

int main(int argc, char **argv) {
  int                serv_sock, clnt_sock;  // socket fd
  struct sockaddr_in serv_adr, clnt_adr;    // socket address(IPv4)
  struct timeval     timeout;               // timeout for select()
  fd_set             reads, cpy_reads;      // fd_set for select()

  socklen_t adr_sz;  // on return, It will contain the actual length of address
  int       fd_max, str_len, fd_num, i;
  char      buf[BUF_SIZE];

  if (argc != 2) {
    printf("Usage: %s <port>\n", argv[0]);
    exit(1);
  }

  // create socket
  serv_sock = socket(PF_INET, SOCK_STREAM, 0);
  memset(&serv_adr, 0, sizeof(serv_adr));
  serv_adr.sin_family = AF_INET;
  serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_adr.sin_port = htons(atoi(argv[1]));

  // bind a name(address) to an unnamed socket.
  if (bind(serv_sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) == -1)
    error_handling("bind() error");
  // listen for connections on a socket
  if (listen(serv_sock, 5) == -1)
    error_handling("listen() error");

  FD_ZERO(&reads);            // set reads to the null set
  FD_SET(serv_sock, &reads);  // include serv_sock in reads
  fd_max = serv_sock;         // check max fd to use in select()

  while (1) {
    cpy_reads = reads;
    timeout.tv_sec = 5;
    timeout.tv_usec = 5000;

    if ((fd_num = select(fd_max + 1, &cpy_reads, 0, 0, &timeout)) == -1)
      break;
    if (fd_num == 0)
      continue;
    for (i = 0; i < fd_max + 1; i++) {
      if (FD_ISSET(i, &cpy_reads)) {
        if (i == serv_sock) {
          adr_sz = sizeof(clnt_adr);
          clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_adr, &adr_sz);
          FD_SET(clnt_sock, &reads);
          if (fd_max < clnt_sock)
            fd_max = clnt_sock;
          printf("connected client: %d\n", clnt_sock);
        } else {
          str_len = read(i, buf, BUF_SIZE);
          if (str_len == 0) {
            FD_CLR(i, &reads);
            close(i);
            printf("closed client: %d\n", i);
          } else {
            write(1, buf, str_len);
            write(i, buf, str_len);
          }
        }
      }
    }
  }
  close(serv_sock);
  return 0;
}

void error_handling(char *buf) {
  fputs(buf, stderr);
  fputc('\n', stderr);
  exit(1);
}