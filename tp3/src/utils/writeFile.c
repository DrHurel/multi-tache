#include "utils.h"
#include <stdio.h>
#include <unistd.h>

int writeFile(const int *tube, FILE *fd) {
  printf("writeFile\n");
  char buf[500];
  size_t r = read(tube[0], buf, 500);
  while (r > 0) {
    printf("writeFile: %ld\n", r);
    fwrite(buf, 1, r, fd);
    r = read(tube[0], buf, 500);
  }

  close(tube[0]);
  return 1;
}