#ifndef JOBS_H
#define JOBS_H

#include "image.h"

#define SMOOSH 0b1
#define COLOR_MUTATE 0b10
#define OTHERS 0b100

typedef struct {
  unsigned char code;
  unsigned int nZone;
} Jobconfig;

union semun {
  int val;
  struct semid_ds *buf;
  unsigned short *array;
}; // creation d'un union semun

void smoosh(Image *image, short id, int nZone, int semid);

void colorMutate(Image *image, short id, int nZone, int semid);

void others(Image *image, short id, int nZone, int semid);

int jobCount(const Jobconfig *config);

void applyJobs(Image *image, const Jobconfig *config, int clesem);

#endif // JOBS_H