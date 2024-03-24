#include "utils.h"
#include <stdio.h>
#include <sys/socket.h>

int sendTCP(int sock, const void *msg, long sizeMsg) {

  int count = 0;

  while (count != sizeMsg) {
    ssize_t res = send(sock, msg + count, sizeMsg - count, 0);
    if (res < 1) { // error or closed
      return res < 0 ? -1 : 0;
    }

    count += res;
  }

  return 1;
}
