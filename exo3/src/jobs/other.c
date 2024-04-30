#include "jobs.h"
#include "sync.h"
#include <stdio.h>
#include <sys/sem.h>
#include <unistd.h>
void others(Image *image, short id, int nZone, int semid) {

  for (short zone = 1; zone <= nZone; zone++) {
    if (id == 0) {
      printf("dont wait\n");
    } else {
      waitFor(semid, id - 1, zone);
      printf("restart %d\n", id + 1);
    }
    sleep(2);
    signalZone(semid, id, zone);
  }

  printf("others %d\n", id);
}