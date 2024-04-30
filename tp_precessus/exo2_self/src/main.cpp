#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

  if (argc != 5) {
    printf("Nbre d'args invalide, utilisation :\n");
    printf("%s <nombre-semaphores> <valeur-initiale> <fichier-pour-cle-ipc> "
           "<entier-pour-clé-ipc>\n",
           argv[0]);
    exit(0);
  }

  int clesem = ftok(argv[3], atoi(argv[4]));

  int nbSem = atoi(argv[1]);

  int idSem = semget(clesem, nbSem, IPC_CREAT | IPC_EXCL | 0600);

  if (idSem == -1) {
    perror("erreur semget : ");
    exit(-1);
  }

  printf("sem id : %d \n", idSem);
}
