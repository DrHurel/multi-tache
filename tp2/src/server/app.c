#include "../../include/server.h"
#include "utils.h"
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

  struct sockaddr_in adc;
  socklen_t adc_len = sizeof(adc);
  int dsClient = accept(ds, (struct sockaddr *)&adc, &adc_len);

  if (dsClient == -1) {
    perror("Serveur : erreur connection : ");
    exit(EXIT_FAILURE);
  } else {
  }

  char buf[4000];
  size_t expected_size;

  if (recvTCP(dsClient, &expected_size, sizeof(size_t)) < 1) {
    perror("err : ");
    exit(EXIT_FAILURE);
  }

  int check = recvTCP(dsClient, buf, (int)expected_size);
  switch (check) {
  case 0:
    printf("Connection fermé");
    close(ds);
    exit(EXIT_FAILURE);
    break;
  case -1:
    perror("Client : echec de la recv:");
    close(ds);
    exit(EXIT_FAILURE);
    break;
  default:
    printf("recive : %zd : %s\n", expected_size, buf);

    break;
  }

  check = sendTCP(dsClient, &expected_size, sizeof(size_t));
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

  /* Etape 6 : fermer la socket (lorsqu'elle n'est plus utilisée)*/
  close(ds);
  printf("Serveur : je termine\n");

  return 0;
}

int run_server_loop(int port) {

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

  struct sockaddr_in adc;
  socklen_t adc_len = sizeof(adc);
  int dsClient = accept(ds, (struct sockaddr *)&adc, &adc_len);

  if (dsClient == -1) {
    perror("Serveur : erreur connection : ");
    exit(EXIT_FAILURE);
  }

  char buf[4000];
  size_t expected_size;

  recv(dsClient, &expected_size, sizeof(size_t), 0);
  while (1) {

    if (recvTCP(dsClient, &expected_size, sizeof(size_t)) < 1) {
      perror("err : ");
      exit(EXIT_FAILURE);
    }

    int check = recvTCP(dsClient, buf, (int)expected_size);
    switch (check) {
    case 0:
      printf("Connection fermé");
      close(ds);
      exit(EXIT_FAILURE);
      break;
    case -1:
      perror("Client : echec de la recv:");
      close(ds);
      exit(EXIT_FAILURE);
      break;
    default:
      printf("recive : %zd : %s\n", expected_size, buf);
      break;
    }

    check = sendTCP(dsClient, &expected_size, sizeof(size_t));
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
  }

  /* Etape 6 : fermer la socket (lorsqu'elle n'est plus utilisée)*/
  close(ds);

  printf("Serveur : je termine\n");

  return 0;
}

int run_server_it(int port) {

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

  struct sockaddr_in adc;
  socklen_t adc_len = sizeof(adc);

  while (1) {
    int dsClient = accept(ds, (struct sockaddr *)&adc, &adc_len);

    if (dsClient == -1) {
      perror("Serveur : erreur connection : ");
      exit(EXIT_FAILURE);
    }

    char buf[4000];
    size_t expected_size;
    int check;
    // récupération de la taille du prochain message
    if ((check = recvTCP(dsClient, &expected_size, sizeof(size_t))) < 0) {
      perror("err : ");
      close(dsClient);

      continue;
    }
    if (check == 0) {
      printf("Connection fermé");
      close(dsClient);

      continue;
    }

    if (expected_size > 4000) { // message trop grand
      close(dsClient);
      printf("oversize message");
      continue;
    }

    check = recvTCP(dsClient, buf, (int)expected_size);
    if (check == 0) {

      printf("Connection fermé");
      close(dsClient);
      continue;
    }
    if (check == -1) {
      perror("Client : echec de la recv:");
      close(dsClient);
      continue;
    }

    printf("recive : %zd : %s\n", expected_size, buf);

    check = sendTCP(dsClient, &expected_size, sizeof(size_t));
    if (check == 0) {
      printf("Connection fermé");
      close(dsClient);
      continue;
    }
    if (check == -1) {
      perror("Client : echec de l'envoi:");
      continue;
    }

    close(dsClient);
    printf("Message bien envoyé");
  }

  /* Etape 6 : fermer la socket (lorsqu'elle n'est plus utilisée)*/
  close(ds);

  printf("Serveur : je termine\n");

  return 0;
}

int run_server_co(int port) {

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
  int pid;
  if (bd == -1) {
    perror("Serveur : pb creation socket :");
    exit(1); // je choisis ici d'arrêter le programme car le reste
             // dépendent de la réussite de la création de la socket.
  }

  if (listen(ds, 10) == -1) {
    perror("Serveur : pb d'écoute :");
    exit(1);
  }

  struct sockaddr_in adc;
  socklen_t adc_len = sizeof(adc);

  while (1) {
    int dsClient = accept(ds, (struct sockaddr *)&adc, &adc_len);

    if (dsClient == -1) {
      perror("Serveur : erreur connection : ");
      exit(EXIT_FAILURE);
    }

    if ((pid = fork()) == -1) { // fork failed
      close(dsClient);
      perror("fail to fork: ");
      continue;
    } else if (pid > 0) { // parent process
      close(dsClient);

    } else if (pid == 0) {
      char buf[4000];
      size_t expected_size;
      if (recvTCP(dsClient, &expected_size, sizeof(size_t)) < 1) {
        perror("err : ");
        exit(EXIT_FAILURE);
      }

      if (expected_size > 4000) {
        printf("oversize message");
        close(dsClient);
        return 0;
      }

      int check = recvTCP(dsClient, buf, (int)expected_size);
      switch (check) {
      case 0:
        printf("Connection fermé");
        close(ds);
        return -1;
        break;
      case -1:
        perror("Client : echec de la recv:");
        close(ds);
        return -1;
        break;
      default:
        printf("recive : %zd : %s\n", expected_size, buf);
        break;
      }

      check = sendTCP(dsClient, &expected_size, sizeof(size_t));
      switch (check) {
      case 0:
        printf("Connection fermé");
        return -1;
        break;
      case -1:
        perror("Client : echec de l'envoi:");
        return -1;
        break;
      default:
        printf("Message bien envoyé");
        break;
      }
      /* Etape 6 : fermer la socket (lorsqu'elle n'est plus utilisée)*/
      close(dsClient);
      return -1;
    }
  }
  close(ds);
  printf("Serveur : je termine\n");

  return 0;
}
