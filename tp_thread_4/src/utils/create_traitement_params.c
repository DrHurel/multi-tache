#include "utils.h"
#include <pthread.h>
#include <stdlib.h>

varPartagees *create_shared(int nb_zone, int nb_traitement) {
  const int nb_var = nb_traitement - 1;
  varPartagees *shared = malloc(sizeof(varPartagees));

  shared->cond = malloc(sizeof(pthread_cond_t) * nb_var);
  shared->di = malloc(sizeof(int) * nb_var);
  shared->nbZones = nb_zone;
  shared->nbTraitements = nb_traitement;
  pthread_mutex_init(&shared->lock, NULL);

  for (int i = 0; i < nb_var; i++) {
    shared->di[i] = 0;
    pthread_cond_init(&shared->cond[i], NULL);
  }

  return shared;
}