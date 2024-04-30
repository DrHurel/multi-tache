#include "sync.h"
#include <stdio.h>
#include <sys/sem.h>
void waitFor(int semid, short target, short zone) {
  printf("wait for zone %d to finish job %d\n", zone, target);
  struct sembuf op = {target, -zone, 0};

  if (semop(semid, &op, 1) == -1) {
    perror("fail :");
  }
}