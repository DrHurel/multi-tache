
#include <pthread.h>
#include <stdio.h> //perror
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

typedef struct {

  int idThread;
  int val;
  int count;
} paramsFonctionThread;

void *fonctionThread(void *params) {

  paramsFonctionThread *args = (paramsFonctionThread *)params;
  args->count++;
  printf("Start Thread %d with value %d %d\n", args->idThread, args->val,
         args->count);

  sleep(args->val);

  printf("End Thread %d with value %d\n", args->idThread, args->val);
  free(params);
  return NULL;
}

int main(int argc, char *argv[]) {

  if (argc < 2) {
    printf("utilisation: %s  nombre_threads  \n", argv[0]);
    return 1;
  }

  printf("start\n");
  printf("%s\n", argv[1]);
  int val = atoi(argv[1]);
  printf("val : %d\n", val);
  pthread_t threads[val];
  printf("init thread tab");
  // création des threards

  for (int i = 0; i < val; i++) {

    // Le passage de param�tre est fortement conseill� (�viter les
    // variables globles).

    paramsFonctionThread *params = malloc(sizeof(paramsFonctionThread));

    if (params == NULL) {
      perror("fail malloc :");
      exit(EXIT_FAILURE);
    } else {
      params->idThread = i;
      params->val = 10 - i;
    }

    // compléter pour initialiser les paramétres
    if (pthread_create(&threads[i], NULL, fonctionThread, params) != 0) {
      perror("erreur creation thread");
      exit(1);
    }
  }

  // garder cette saisie et modifier le code en temps venu.
  printf("saisir un caract�re \n");
  fgetc(stdin);
  return 0;
}
