#include "server.h"
#include <stdio.h>
#include <sys/socket.h>

int init_server(int port, int max_concurent_connect) {
  int ds = socket(PF_INET, SOCK_STREAM, 0);

  if (ds < 0) {
    perror("fail to create socket:");
    return -1;
  }

  printf("Serveur TPC : creation de la socket réussie \n");

  /* Etape 2 : Nommer la socket du seveur */
  struct sockaddr_in ade;           // structure d'adresse du serveur
  ade.sin_family = AF_INET;         // famille d'adresse
  ade.sin_addr.s_addr = INADDR_ANY; // toutes les interfaces locales disponibles
  ade.sin_port = htons((short)port); // port d'écoute
  int bd = bind(ds, (struct sockaddr *)&ade,
                sizeof(struct sockaddr_in)); // liaison de la socket avec la
                                             // structure d'adresse

  if (bd < 0) {
    return -2;
  }

  if (listen(ds, max_concurent_connect) < 0) {
    return -3;
  }

  return ds;
}