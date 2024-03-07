#include "utils.h"
#include <stdio.h>
#include <sys/socket.h>

int recvTCP(int sock, void *msg, long sizeMsg) {
  int count = 0;

  while (count != sizeMsg) {
    ssize_t res = recv(sock, msg + count, sizeMsg - count, 0);
    if (res < 1) {
      return res < 0 ? -1 : 0;
    }

    count += res;
  }

  return 1;
}