#include "client.h"
#include "utils.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

// création et connexion du client
int init_client(const char *ip, uint16_t port) {
  // create socket tcp
  int ds = socket(PF_INET, SOCK_STREAM, 0);
  if (ds < 0) {
    return -1;
  }

  // adress of server
  struct sockaddr_in adr;
  adr.sin_family = AF_INET;
  adr.sin_port = htons(port);

  adr.sin_addr.s_addr = inet_addr(ip);

  // connect to server
  if (connect(ds, (struct sockaddr *)&adr, sizeof(adr)) < 0) {
    perror("fail to connect:");
    close(ds);
    return -1;
  }

  return ds;
}

int run_client(const char *ip, uint16_t port, const char *file_name) {

  int ds = init_client(ip, port);
  if (ds < 0) {
    exit(EXIT_FAILURE);
  }

  FILE *fd = fopen(file_name, "rd");
  if (fd == NULL) {
    perror("fail to open file:");
    close(ds);
    return -1;
  }

  long filezize = file_size(fd, file_name);
  if (filezize < 0) {
    perror("fail to get file size:");
    close(ds);
    return -1;
  }

  ssize_t res = sendTCP(ds, &filezize, sizeof(long));
  if (res < 0) {
    perror("fail to send file size:");
    close(ds);
    return -1;
  }

  char buf[500]; // buffer for data
  long data_sent = 0;
  printf("file size: %ld\n", filezize);

  // send file name size
  size_t file_name_size = strlen(file_name);
  res = sendTCP(ds, &file_name_size, sizeof(size_t));
  if (res < 0) {
    perror("fail to send file name size:");
    close(ds);
    return -1;
  }

  // send file name
  res = sendTCP(ds, file_name, file_name_size);
  if (res < 0) {
    perror("fail to send file name:");
    close(ds);
    return -1;
  }

  while (data_sent < filezize) {
    size_t r = fread(buf, 1, 500, fd);
    res = sendTCP(ds, buf, r);
    data_sent += r;
    if (res < 0) {
      perror("fail to send data:");
      close(ds);
      return -1;
    }
  }

  close(ds);
  return 1;
}
