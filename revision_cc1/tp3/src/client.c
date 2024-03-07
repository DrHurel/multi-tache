#include "client.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int start_client(const char *ip, const uint16_t port) {
  if (port < 1024) {
    printf("Erreur port %d est invalide\n", port);
    return -1;
  }

  printf("Connection au serveur %s:%d\n", ip, port);
  /* Etape 1 : créer une socket */
  int ds = socket(PF_INET, SOCK_DGRAM, 0);

  /* /!\ : Il est indispensable de tester les valeurs de retour de
     toutes les fonctions et agir en fonction des valeurs
     possibles. Voici un exemple */
  if (ds == -1) {
    perror("Client : pb creation socket :");
    return -1;
  }

  /* J'ajoute des traces pour comprendre l'exécution et savoir
     localiser des éventuelles erreurs */
  printf("Client : creation de la socket réussie \n");

  /* Etape 3 : Désigner la socket du serveur */
  struct sockaddr_in ads;
  ads.sin_family = AF_INET;
  ads.sin_addr.s_addr = inet_addr(ip);
  ads.sin_port = htons(port);

  return 1;
};

void init_client() {
  char ip[10];
  int port;

  do {
    printf("Renseigner l'ip du serveur sur lequel se connecter : \n");
    scanf("%9s", ip);
    printf("Renseigner le port de la socket : \n");
    char temp[5];
    scanf("%4s", temp);
    port = atoi(temp);

    // setup socket

  } while (start_client(ip, port) < 0);

  printf("Fin\n");
}
