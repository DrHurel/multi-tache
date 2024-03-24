#include "client.h"
#include "server.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
  char mode;
  uint16_t port = 3000;

  do {

    printf("Demarrer un serveur ou un client [s/c]?\n");
    scanf("%c", &mode);

  } while (mode != 'c' && mode != 's');

  if (mode == 'c') {
    char *ip = malloc(16);
    char *file_name = malloc(256);

    printf("ip: ");
    scanf("%s", ip);
    printf("port: ");
    scanf("%hu", &port);
    printf("file name: ");
    scanf("%s", file_name);
    run_client(ip, port, file_name);
    free(ip);
    free(file_name);
  } else {

    printf("port: ");
    scanf("%hu", &port);
    printf("max concurent connect: ");
    int max_concurent_connect;
    scanf("%d", &max_concurent_connect);
    run_server(port, max_concurent_connect);
  }

  return 0;
}
