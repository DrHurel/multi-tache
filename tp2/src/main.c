#include "../include/client.h"
#include "../include/server.h"
#include "../include/ui.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

  UI_CONFIG config = loadConfig(argv, argc);

  printf("mode : %d\n", config.mode);
  printf("port : %d\n", config.port);
  if (config.mode == CLIENT) {
    printf("ip : %s\n", config.ip);
  }

  switch (config.mode) {
  case SERVER:
    run_server(config.port);
    break;
  case CLIENT:
    run_client(config.ip, config.port);
    break;
  case SERVER_LOOP:
    run_server_loop(config.port);
    break;
  case CLIENT_LOOP:

    run_client_loop(config.ip, config.port);
    break;
  default:
    exit(EXIT_FAILURE);
  }

  return 0;
}
