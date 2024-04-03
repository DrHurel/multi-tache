#include "utils.h"
#include <bits/pthreadtypes.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void wait_for_traitement(params *p, int id_zone) {
  printf("wait\n");
  pthread_mutex_lock(&(p->vPartage->lock));

  while (p->vPartage->di[p->idThread - 1] < id_zone) {
    printf("wait\n");
    pthread_cond_wait(&(p->vPartage->cond[p->idThread - 1]),
                      &(p->vPartage->lock));
  }

  pthread_mutex_unlock(&(p->vPartage->lock));
}

void increment_count(varPartagees *shared, int target) {
  pthread_mutex_lock(&(shared->lock));
  shared->di[target]++;
  pthread_mutex_unlock(&(shared->lock));
}

void traitement_action(){
    // sleep((rand() % 4) + 1);
};

void *traitement(void *p) {

  params *args = (params *)p;
  varPartagees *vPartage = args->vPartage;
  const int nb_zone = vPartage->nbZones;
  for (int i = 0; i < nb_zone; i++) {
    if (args->idThread != 0) {

      wait_for_traitement(args, i + 1);
    }
    printf("thread %d debut zone %d\n", args->idThread, i);
    traitement_action();
    printf("log:%d :::::: zone %d\n", args->idThread, i);

    if (args->idThread == args->vPartage->nbTraitements - 1) {
      continue;
    }
    increment_count(vPartage, args->idThread);
    pthread_cond_signal(&(vPartage->cond[args->idThread]));
  }
  pthread_exit(NULL);
}
