#ifndef UTILS_H
#define UTILS_H

// structure qui regroupe les variables partagées entre les threads.
#include <pthread.h>
typedef struct {
  int nbZones;
  int nbTraitements;
  int *di; // le tableau indiqué dans l'énoncé
  pthread_mutex_t lock;
  pthread_cond_t *cond;
  //
} varPartagees;

// structure qui regroupe les paramétres d'un thread
typedef struct {

  int idThread; // cet entier correspond au numéro de traitement associé à un
                // thread

  varPartagees *vPartage;

} params;

varPartagees *create_shared(int nb_zone, int nb_traitement);
void *traitement(void *p);
#endif // UTILS_H