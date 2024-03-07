
#include "server.h"
#include <arpa/inet.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int start_server(uint16_t port) {

  if (port < 1024) {
    printf("Erreur port %d est invalide\n", port);
    return -1;
  }

  int ds = socket(PF_INET, SOCK_DGRAM, 0);

  if (ds < 0) {
    perror("Serveur : pb creation socket :");
    return -1;
  }
  printf("Serveur : creation de la socket réussie \n");
  struct sockaddr_in ad;
  ad.sin_family = PF_INET;
  ad.sin_addr.s_addr = INADDR_ANY;
  ad.sin_port = htons(port);
  int bd = bind(ds, (struct sockaddr *)&ad, sizeof(ad));

  if (bd == -1) {
    perror("Serveur : pb nomage socket :");
    return -1;
  }
  printf("Serveur : nomage de la socket réussie sur le port %d\n", port);

  do {
    recvfrom(ds, void *restrict buf, size_t n, int flags,
             struct sockaddr *restrict addr, socklen_t *restrict addr_len)
  } while (1)

      return 1;
}

void init_server() {

  uint16_t port;
  do {

    char temp[5];
    printf("\nRenseigner le port de la socket : \n");
    scanf("%4s", temp);
    port = (uint16_t)atoi(temp);

    // setup socket

  } while (start_server(port) < 0);

  printf("Fin\n");
}