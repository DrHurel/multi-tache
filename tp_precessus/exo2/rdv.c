#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

  if (argc != 3) {
    printf("Nbre d'args invalide, utilisation :\n");
    printf("%s fichier-pour-cle-ipc entier-pour-cl�-ipc\n", argv[0]);
    exit(0);
  }

  int clesem = ftok(argv[1], atoi(argv[2]));

  int nbSem = 1;

  int idSem =
      semget(clesem, nbSem, IPC_CREAT | 0600); // accède a la valeur présente

  if (idSem == -1) {
    perror("erreur semget : ");
    exit(-1);
  }

  printf("sem id : %d \n", idSem);

  // on récupe le semaphore

  union semun {
    int val;
    struct semid_ds *buf;
    ushort *array;
  }; // creation d'un union semun

  union semun valinit;

  valinit.array = (ushort *)malloc(
      nbSem * sizeof(ushort)); // pour montrer qu'on r�cup�re bien un nouveau
                               // tableau dans la suite

  if (semctl(idSem, nbSem, GETALL, valinit) == -1) {
    perror("erreur initialisation sem : ");
    exit(1);
  }

  printf("Valeur du sempahore récupéré [ ");
  for (int i = 0; i < nbSem - 1; i++) {
    printf("%d, ", valinit.array[i]);
  }
  printf("%d ] \n", valinit.array[nbSem - 1]);

  struct sembuf op[] = {
      // d'après docteur Hurel il faut retirer les casts
      {(u_short)0, (short)-1, 0},
      {(u_short)0, (short)+1, 0},
      {(u_short)0, (short)0, 0}}; // op[0] == P   op[1] == V   op[2] == Z

  // pov je décrémente la valeur du sem  donc on PREND c'est P
  printf("Je prend 1 dans la valeurs du sem\n");
  if (semop(idSem, op, 1) == -1) {
    perror("erreur semop P : ");
    exit(1);
  }

  if (semctl(idSem, nbSem, GETALL, valinit) == -1) {
    perror("erreur initialisation sem : ");
    exit(1);
  }
  printf("Valeur du sempahore [ ");
  for (int i = 0; i < nbSem - 1; i++) {
    printf("%d, ", valinit.array[i]);
  }
  printf("%d ] \n", valinit.array[nbSem - 1]);

  // pov j'attend que la valeur du sem soit a 0 c'est Z
  printf("J'attend que la valeur du sem soit a 0\n");

  if (semop(idSem, op + 2, 1) == -1) {
    perror("erreur semop Z : ");
    exit(1);
  }

  if (semctl(idSem, nbSem, GETALL, valinit) == -1) {
    perror("erreur initialisation sem : ");
    exit(1);
  }

  printf("Valeur du sempahore [ ");
  for (int i = 0; i < nbSem - 1; i++) {
    printf("%d, ", valinit.array[i]);
  }
  printf("%d ] \n", valinit.array[nbSem - 1]);

  printf("point de rdv attein\n");

  free(valinit.array);
  return 0;
}
