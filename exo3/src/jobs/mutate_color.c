#include "jobs.h"
#include "sync.h"
#include <stdio.h>
#include <unistd.h>
void colorMutate(Image *image, short id, int nZone, int semid) {
  for (short zone = 1; zone <= nZone; zone++) {
    if (id == 0) {
      printf("dont wait\n");
    } else {
      waitFor(semid, id - 1, zone);
    }

    for (int i = (image->height * image->width) * (zone - 1) / nZone;
         i < (image->height * image->width) * zone / nZone; i++) {
      image->ref[i] = (image->ref[i] - 122) % 255;
    }

    signalZone(semid, id, zone);
  }
  printf("mutate %d\n", id);
}