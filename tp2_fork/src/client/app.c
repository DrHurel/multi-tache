#include "../../include/client.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int run_client(const char *ip, int port) {
  int ds = socket(PF_INET, SOCK_STREAM, 0);

  if (ds == -1) {
    perror("Client : pb creation socket :");
    exit(1); // je choisis ici d'arrêter le programme car le reste
             // dépendent de la réussite de la création de la socket.
  }

  printf("Client TPC : creation de la socket réussie \n");

  /* Etape 2 : Nommer la socket du seveur */
  struct sockaddr_in ad;
  ad.sin_family = PF_INET;
  ad.sin_addr.s_addr = inet_addr(ip);
  ad.sin_port = htons((short)port);

  if (connect(ds, (struct sockaddr *)&ad, sizeof(ad)) == -1) {
    perror("Client : erreur connect :");
    exit(1);
  }

  printf("Client : connexion établie\n");
  char data[100];
  fgets(data, 100, stdin);

  size_t dataSize = strlen(data) - 1;
  send(ds, &dataSize, sizeof(size_t), 0);
  send(ds, data, dataSize, 0);
  send(ds, data, dataSize, 0);
  int resMessage;
  recv(ds, &resMessage, sizeof(int), 0);
  printf("message : %d\n", resMessage);

  /* Etape 6 : fermer la socket (lorsqu'elle n'est plus utilisée)*/
  close(ds);

  printf("Client : je termine\n");

  return 0;
}
