#include "utils.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

// fonction associée à chaque thread secondaire à créer.

int main(const int argc, const char **argv) {
  if (argc != 3) {
    printf("usage : %s <nb_traitement> <nb_zone>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  int nb_traitement = atoi(argv[1]);
  int nb_zone = atoi(argv[2]);

  printf("init shared\n");
  varPartagees *shared = create_shared(nb_zone, nb_traitement);
  if (shared == NULL) {
    perror("fail init shared:");
    exit(EXIT_FAILURE);
  }
  printf("init all thread\n");

  for (int i = 0; i < nb_traitement; i++) {
    pthread_t *t = malloc(sizeof(pthread_t));
    params *payload = malloc(sizeof(params));
    payload->idThread = i;
    payload->vPartage = shared;
    pthread_create(t, NULL, traitement, payload);
    if (i == nb_traitement - 1) {
      pthread_join(*t, NULL);
    }
  }

  pthread_mutex_destroy(&shared->lock);
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