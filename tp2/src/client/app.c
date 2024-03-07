#include "../../include/client.h"
#include "utils.h"
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
  fgets(data, 100, stdin); // récupération du message depuis la console

  printf("%s\n", data);

  int dataSize = (int)strlen(data) - 1;

  int check = sendTCP(ds, &dataSize, sizeof(int));
  switch (check) {
  case 0:
    printf("Connection fermé");
    exit(EXIT_FAILURE);
    break;
  case -1:
    perror("Client : echec de l'envoi:");
    exit(EXIT_FAILURE);
    break;
  default:
    printf("Message bien envoyé");
    break;
  }
  check = sendTCP(ds, data, dataSize);

  switch (check) {
  case 0:
    printf("Connection fermé");
    exit(EXIT_FAILURE);
    break;
  case -1:
    perror("Client : echec de l'envoi:");
    exit(EXIT_FAILURE);
    break;
  default:
    printf("Message bien envoyé");
    break;
  }

  int resMessage;
  check = recvTCP(ds, &resMessage, sizeof(int));

  switch (check) {
  case 0:
    printf("Connection fermé");
    exit(EXIT_FAILURE);
    break;
  case -1:
    perror("Client : echec de l'envoi:");
    exit(EXIT_FAILURE);
    break;
  default:
    printf("message : %d\n", resMessage);

    break;
  }

  /* Etape 6 : fermer la socket (lorsqu'elle n'est plus utilisée)*/
  close(ds);

  printf("Client : je termine\n");

  return 0;
}

int run_client_loop(const char *ip, int port) {
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

  while (1) {
    fgets(data, 100, stdin);
    if (data[0] == 'Q') {
      printf("exit program");
      break;
    }

    int dataSize = (int)strlen(data) - 1;

    int check = sendTCP(ds, &dataSize, sizeof(int));
    switch (check) {
    case 0:
      printf("Connection fermé");
      exit(EXIT_FAILURE);
      break;
    case -1:
      perror("Client : echec de l'envoi:");
      exit(EXIT_FAILURE);
      break;
    default:
      printf("Message bien envoyé");
      break;
    }
    check = sendTCP(ds, data, dataSize);

    switch (check) {
    case 0:
      printf("Connection fermé");
      exit(EXIT_FAILURE);
      break;
    case -1:
      perror("Client : echec de l'envoi:");
      exit(EXIT_FAILURE);
      break;
    default:
      printf("Message bien envoyé");
      break;
    }

    int resMessage;
    check = recvTCP(ds, &resMessage, sizeof(int));

    switch (check) {
    case 0:
      printf("Connection fermé");
      exit(EXIT_FAILURE);
      break;
    case -1:
      perror("Client : echec de la recv:");
      exit(EXIT_FAILURE);
      break;
    default:
      printf("message : %d\n", resMessage);
      break;
    }
  }
  /* Etape 6 : fermer la socket (lorsqu'elle n'est plus utilisée)*/
  close(ds);

  printf("Client : je termine\n");

  return 0;
}