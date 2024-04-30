
#include "image.h"
#include "image_ppm.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>

void createImage(const char *ref, Image *res, int cle) {

  // load Image

  int h;
  int w;
  lire_nb_lignes_colonnes_image_pgm(ref, &h, &w);

  OCTET *pt_image = malloc(sizeof(char) * h * w);

  int laMem = shmget(cle, h * w * sizeof(char), IPC_CREAT | IPC_EXCL | 0600);
  if (laMem == -1) {
    perror("erreur shmget : ");
    exit(-1);
  }

  lire_image_pgm(ref, pt_image, h * w);

  printf("creation segment de m�moire ok. mem id : %d \n", laMem);

  res->height = h;
  res->width = w;
  // attachement au segment pour pouvoir y acc�der
  res->ref = (char *)shmat(laMem, NULL, 0);
  if (res->ref == (char *)-1) {
    perror("erreur shmat : ");
    exit(-1);
  }

  // j'ai un pointeur sur le segment, j'initialise le tableau

  for (int i = 0; i < h * w; i++) {
    res->ref[i] = pt_image[i];
  }
}