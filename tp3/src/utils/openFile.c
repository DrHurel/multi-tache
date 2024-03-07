#include "utils.h"
#include <stdio.h>

// ouvre un fichier et retourne la taille de celui-ci
long file_size(FILE *fd, const char *path) {

  if (fd != NULL) {
    fseek(fd, 0L, SEEK_END);
    long sz = ftell(fd);
    rewind(fd);

    return sz;
  } else {
    return -1;
  }
}
