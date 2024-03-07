#include "client.h"
#include "server.h"
#include <stdio.h>

int main() {
  char mode;

  do {

    printf("Demarrer un serveur ou un client [s/c]?\n");
    scanf("%c", &mode);

  } while (mode != 'c' && mode != 's');

  if (mode == 'c') {
    init_client();
  } else {
    init_server();
  }

  return 0;
}
