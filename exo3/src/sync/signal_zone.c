#include "sync.h"
#include <stdio.h>
#include <sys/sem.h>
void signalZone(int semid, short target, short zone) {
  printf("signal zone %d that job %d is done\n", zone, target);
  struct sembuf op = {target, zone, 0};

  if (semop(semid, &op, 1) == -1) {
    perror("fail :");
  }
}