
#include "image.h"
#include "jobs.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void applyJobs(Image *image, const Jobconfig *config, int clesem) {
  int status = 0;
  printf("apply Job\n");
  int n = jobCount(config);
  printf("job count : %d\n", n);
  int nbSem = n; // copy in case

  int idSem =
      semget(clesem, nbSem, IPC_CREAT | 0600); // accède a la valeur présente

  if (idSem == -1) {
    perror("erreur semget : ");
    exit(-1);
  }

  printf("sem id : %d \n", idSem);

  // on récupe le semaphore

  int nZone = 10;

  union semun valinit;

  valinit.array = (ushort *)malloc(
      nbSem - 1 * sizeof(ushort)); // pour montrer qu'on r�cup�re bien un
                                   // nouveau tableau dans la suite

  for (int i = 0; i < nbSem; i++) {

    valinit.array[i] = 0;
  }

  if (semctl(idSem, nbSem, GETALL, valinit) == -1) {
    perror("erreur initialisation sem : ");
    exit(1);
  }

  for (unsigned int i = 1; i <= 256; i *= 2) {
    if (n == 0) { // no more job to start
      break;
    }
    switch (config->code & i) {
    case SMOOSH:
      if (fork() == 0) {
        smoosh(image, nbSem - n, nZone, idSem);

        exit(EXIT_SUCCESS);
      } else {
        --n;
      }

    case COLOR_MUTATE:
      if (fork() == 0) {
        colorMutate(image, nbSem - n, nZone, idSem);

        exit(EXIT_SUCCESS);
      } else {
        --n;
      }

    case OTHERS:
      if (fork() == 0) {
        others(image, nbSem - n, nZone, idSem);

        exit(EXIT_SUCCESS);
      } else {
        --n;
      }

    default:
      continue;
    }
  }

  while (wait(&status) > 0)
    ;

  return;
}
