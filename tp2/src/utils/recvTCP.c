#include "utils.h"
#include <sys/socket.h>

int recvTCP(int sock, void *msg, int sizeMsg) {
  int count = 0;

  while (count != sizeMsg) {
    int res = (int)recv(sock, msg + count, sizeMsg - count, 0);
    if (res < 1) {
      return res;
    }

    count += res;
  }

  return 1;
}