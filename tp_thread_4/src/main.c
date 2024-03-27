#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

// structure qui regroupe les variables partagées entre les threads.
typedef struct {
  int nbZones;
  int *di; // le tableau indiqué dans l'énoncé
  pthread_mutex_t lock;
  pthread_cond_t cond;
  //
} varPartagees;

// structure qui regroupe les paramétres d'un thread
typedef struct {

  int idThread; // cet entier correspond au numéro de traitement associé à un
                // thread

  varPartagees *vPartage;
} params;

// fonction associée à chaque thread secondaire à créer.

void *traitement(void *p) {

  params *args = (params *)p;
  varPartagees *vPartage = args->vPartage;
  for (int i = 0; i < vPartage->nbZones; i++) {

    if (args->idThread != 1) { // le premier traitement n'attent personne
      pthread_mutex_lock(&vPartage->lock);
      while (vPartage->di[i] < args->idThread - 1) {
        pthread_cond_wait(&vPartage->cond, &vPartage->lock);
      }

      pthread_mutex_unlock(&vPartage->lock);
    }
    printf("debut etape %d thread %d\n", i, args->idThread);

    int time = rand() % 10;
    for (int j = 0; j < time; j++) {
      sleep(1);
    }

    // a la fin du traitement d'une zone, le signaler pour qu'un thread en
    // attente se réveille.
    printf("etape %d thread %d fini\n", i, args->idThread);
    pthread_mutex_lock(&vPartage->lock);
    vPartage->di[i]++;
    pthread_mutex_unlock(&vPartage->lock);
    pthread_cond_broadcast(&vPartage->cond);
  }
  pthread_exit(NULL);
}

int main(const int argc, const char **argv) {
  if (argc != 3) {
    printf("usage : %s <nb_traitement> <target>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  int nb_traitement = atoi(argv[1]);

  printf("init shared\n");
  varPartagees *shared = malloc(sizeof(varPartagees));
  shared->nbZones = nb_traitement;
  shared->di = (int *)malloc(sizeof(int) * nb_traitement);
  printf("malloc\n");

  pthread_mutex_init(&shared->lock, NULL);
  pthread_cond_init(&shared->cond, NULL);

  printf("init all thread\n");
  for (int i = 1; i <= nb_traitement; i++) {
    shared->di[i] = 0;
  }
  for (int i = 1; i <= nb_traitement; i++) {
    pthread_t *t = malloc(sizeof(pthread_t));
    params *payload = malloc(sizeof(params));
    payload->idThread = i;
    payload->vPartage = shared;
    pthread_create(t, NULL, traitement, payload);
    if (i == nb_traitement) {
      pthread_join(*t, NULL);
    }
  }

  free(shared->di);
  free(shared);
  return 0;
}

/*
    -[  1  ]-[  2  ]-[  3  ]-[  4  ]->
th 1 [start]-[ ... ]-[ ... ]-[ ... ]
th 2 [wait1]-[ ... ]-[ ... ]-[ ... ]
th 3 [wait2]-[ ... ]-[ ... ]-[ ... ]
th 4 [wait3]-[ ... ]-[ ... ]-[ ... ]

    -[  1  ]-[  2  ]-[  3  ]-[  4  ]->
th 1 [ end ]-[start]-[ ... ]-[ ... ]
th 2 [start]-[wait1]-[ ... ]-[ ... ]
th 3 [wait2]-[ ... ]-[ ... ]-[ ... ]
th 4 [wait3]-[ ... ]-[ ... ]-[ ... ]

    -[  1  ]-[  2  ]-[  3  ]-[  4  ]->
th 1 [ end ]-[ end ]-[start]-[ ... ]
th 2 [ end ]-[start]-[wait1]-[ ... ]
th 3 [start]-[wait2]-[ ... ]-[ ... ]
th 4 [wait3]-[ ... ]-[ ... ]-[ ... ]

    -[  1  ]-[  2  ]-[  3  ]-[  4  ]->
th 1 [ end ]-[ end ]-[ end ]-[start]
th 2 [ end ]-[ end ]-[start]-[wait1]
th 3 [ end ]-[start]-[ ... ]-[ ... ]
th 4 [start]-[wait3]-[ ... ]-[ ... ]

    -[  1  ]-[  2  ]-[  3  ]-[  4  ]->
th 1 [ end ]-[ end ]-[ end ]-[ end ]
th 2 [ end ]-[ end ]-[ end ]-[start]
th 3 [ end ]-[ end ]-[start]-[wait2]
th 4 [ end ]-[start]-[wait3]-[ ... ]

    -[  1  ]-[  2  ]-[  3  ]-[  4  ]->
th 1 [ end ]-[ end ]-[ end ]-[ end ]
th 2 [ end ]-[ end ]-[ end ]-[ end ]
th 3 [ end ]-[ end ]-[ end ]-[start]
th 4 [ end ]-[ end ]-[start]-[wait3]


    -[  1  ]-[  2  ]-[  3  ]-[  4  ]->
th 1 [ end ]-[ end ]-[ end ]-[ end ]
th 2 [ end ]-[ end ]-[ end ]-[ end ]
th 3 [ end ]-[ end ]-[ end ]-[ end ]
th 4 [ end ]-[ end ]-[ end ]-[start]
*/