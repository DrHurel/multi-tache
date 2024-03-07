#include "../../include/server.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int run_server(int port) {

  int ds = socket(PF_INET, SOCK_STREAM, 0);

  if (ds == -1) {
    perror("Serveur : pb creation socket :");
    exit(1); // je choisis ici d'arrêter le programme car le reste
             // dépendent de la réussite de la création de la socket.
  }

  printf("Serveur TPC : creation de la socket réussie \n");

  /* Etape 2 : Nommer la socket du seveur */
  struct sockaddr_in ad;
  ad.sin_family = PF_INET;
  ad.sin_addr.s_addr = INADDR_ANY;
  ad.sin_port = htons((short)port);
  int bd = bind(ds, (struct sockaddr *)&ad, sizeof(ad));

  if (bd == -1) {
    perror("Serveur : pb creation socket :");
    exit(1); // je choisis ici d'arrêter le programme car le reste
             // dépendent de la réussite de la création de la socket.
  }

  if (listen(ds, 10) == -1) {
    perror("Serveur : pb d'écoute :");
    exit(1);
  }

  while (1) {
    // WIP
  }

  close(ds);

  printf("Serveur : je termine\n");

  return 0;
}
