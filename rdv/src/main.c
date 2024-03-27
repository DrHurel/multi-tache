#include "calcul.h"

#include <bits/pthreadtypes.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

typedef struct {
  pthread_mutex_t lock; // lock for counter
  pthread_cond_t cond;
  int count;

  // regrouoes les donnée partagées entres les threads participants aux RdV :
} predicatRdv;

typedef struct {

  // structure pour regrouper les paramétres d'un thread.
  int idThread; // un identifiant de thread, de 1 é N (N le nombre
                // total de theads secondaires
  predicatRdv *varPartagee;
} params;

// fonction associée a chaque thread participant au RdV.

void *participant(void *p) {

  params *args = (params *)p;
  predicatRdv *predicat = args->varPartagee;

  // simuvlation d'un long calcul pour le travail avant RdV
  printf("start thread %d\n", args->idThread);
  // simulation d'un long calcul pour le travail avant RdV
  calcul(args->idThread + rand() % 10); // c'est un exemple.
  printf("end calc %d\n", args->idThread);
  // RdV
  pthread_mutex_lock(&predicat->lock);

  predicat->count--;
  printf("count :%d\n", predicat->count);
  if (predicat->count) {
    pthread_cond_broadcast(&predicat->cond);
  }
  while (predicat->count > 0) { // attention : le dernier arrivé ne doit pas
    printf("stop %d\n", args->idThread);
    pthread_cond_wait(&predicat->cond,
                      &predicat->lock); // attendre. Il doit
                                        // réveiller tous les autres.
  }
  pthread_mutex_unlock(&predicat->lock);

  printf("reprise %d\n", args->idThread);
  calcul(1); // reprise et poursuite de l'execution.

  pthread_exit(NULL); // fortement recommandé.
}

int main(int argc, char **argv) {
  if (argc != 2) {

    printf("usage %s <nb_particiant>\n", argv[0]);
    return 1;
  }
  int nb_participant = atoi(argv[1]);
  predicatRdv *commun = malloc(sizeof(predicatRdv));
  pthread_t threadList[nb_participant];
  commun->count = nb_participant;

  pthread_mutex_init(&commun->lock, NULL);
  pthread_cond_init(&commun->cond, NULL);

  for (int i = 0; i < nb_participant; i++) {

    params *p = malloc(sizeof(params));

    if (p == NULL) {
      perror("fail malloc :");
      exit(EXIT_FAILURE);
    } else {
      p->idThread = i;
      p->varPartagee = commun;
    }
    pthread_create(&threadList[i], NULL, participant, p);
  }

  for (int i = 0; i < nb_participant; i++) {
    printf("join %d\n", i);
    pthread_join(threadList[i], NULL);
  }

  pthread_mutex_destroy(&commun->lock);
  pthread_cond_destroy(&commun->cond);

  return 0;
}