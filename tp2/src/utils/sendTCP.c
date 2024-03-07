#include "utils.h"
#include <stdio.h>
#include <sys/socket.h>

int sendTCP(int sock, const void *msg, int sizeMsg) {

  int count = 0;

  while (count != sizeMsg) {
    printf("test");
    int res = (int)send(sock, msg + count, sizeMsg - count, 0);
    if (res < 1) { // error or closed
      return res;
    }

    count += res;
  }

  return 1;
}
