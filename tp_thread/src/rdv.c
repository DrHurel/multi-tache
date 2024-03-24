#include "calcul.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

struct predicatRdv {

  // regrouoes les donnée partagées entres les threads participants aux RdV :
};

struct params {

  // structure pour regrouper les param�tres d'un thread.

  int idThread; // un identifiant de thread, de 1 � N (N le nombre
                // total de theads secondaires
  struct predicatRdv *varPartagee;
};

// fonction associ�e a chaque thread participant au RdV.

void *participant(void *p) {

  struct params *args = (struct params *)p;
  struct predicatRdv *predicat = args->varPartagee;

  // simulation d'un long calcul pour le travail avant RdV

  // simulation d'un long calcul pour le travail avant RdV
  calcul(args->idThread + rand() % 10); // c'est un exemple.

  // RdV
  while (1) { // attention : le dernier arriv� ne doit pas attendre. Il doit
              // r�veiller tous les autres.
  }

  calcul(1); // reprise et poursuite de l'execution.

  pthread_exit(NULL); // fortement recommand�.
}
