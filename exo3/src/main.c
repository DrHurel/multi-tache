#include "image.h"
#include "image_ppm.h"
#include "jobs.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
/*
 cr�ation et initialisation d'un tableau de s�maphores pour le
 traitement synchronis�. Le nombre d'�l�ments correspond au nombre de
 traitements -1 et les valeurs initiales sont � 0 (� la case i, la
 valeur corerspond � la derni�re zone trait�e par le processus
 P_(i+1))

 cr�ation d'un segment de memoire partag� qui sera un tableau d'entier (un
 �l�lement correspondra � une zone)
 */

int main(int argc, char *argv[]) {
  int status;
  // max nombre-zone = 127
  if (argc != 5) {
    printf("Nbre d'args invalide, utilisation :\n");
    printf("%s <code_traitement> <nombre-zones> <fichier-pour-cle-ipc>"
           "<entier-pour-cle-ipc>\n",
           argv[0]);
    exit(0);
  }

  int cle = ftok(argv[3], atoi(argv[4]));

  Image img = {0, 0, NULL};
  // d�tachement pour signaler au syst�me la fin de l'utilisation du segment

  Jobconfig config = {SMOOSH | OTHERS | COLOR_MUTATE, atoi(argv[2]) % 128};

  createImage("./assets/1_Aerial.pgm", &img, cle);

  applyJobs(&img, &config, cle);

  printf("all done \n");
  ecrire_image_pgm("test.out.pgm", img.ref, img.height, img.width);
  if (shmdt(img.ref) == -1) {
    perror("erreur shmdt : ");
    exit(-1);
  }
  return 0;
}
