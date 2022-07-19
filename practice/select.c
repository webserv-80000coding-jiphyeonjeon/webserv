#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

#define BUF_SIZE 1024
// if BUF_SIZE is too small, the program terminate sometimes.
// (error occurs on select())

void error_handling(char *buf);

int main(int argc, char **argv) {
  int                serv_sock, clnt_sock;  // socket fd
  struct sockaddr_in serv_adr, clnt_adr;    // socket address(IPv4)
  struct timeval     timeout;               // timeout for select()
  fd_set             reads, cpy_reads;      // fd_set for select()

  socklen_t adr_sz;  // on return, It will contain the actual length of address
  int       fd_max, str_len, fd_num, i, call = 0;
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
    printf("%d\n", call++);
    cpy_reads = reads;  // set cpy_reads to use on select()
    // set timeout
    timeout.tv_sec = 5;
    timeout.tv_usec = 5000;

    // check fd sets to read from fd which is ready for reading
    // 0 to nfds - 1, timeout is maximum interval to wait.
    if ((fd_num = select(fd_max + 1, &cpy_reads, 0, 0, &timeout)) == -1)
      break;
    if (fd_num == 0)  // means time limit is expired
      continue;
    for (i = 0; i < fd_max + 1; i++) {  // check all fd
      if (FD_ISSET(i, &cpy_reads)) {    // if fd is ready for reading
        if (i == serv_sock) {           // if it is a new connection
          adr_sz = sizeof(clnt_adr);    // accept a connection on a socket
          clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_adr, &adr_sz);
          FD_SET(clnt_sock, &reads);  // add new socket to reads
          if (fd_max < clnt_sock)
            fd_max = clnt_sock;
          printf("connected client: %d\n", clnt_sock);
        } else {                             // if it is a data from a client
          str_len = read(i, buf, BUF_SIZE);  // read data from a socket
          if (str_len == 0) {                // if no data to read
            FD_CLR(i, &reads);               // remove fd from read and close the fd
            close(i);
            printf("closed client: %d\n", i);
          } else {
            write(1, buf, str_len);  // print data to stdout
            write(i, buf, str_len);  // send data to a client
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