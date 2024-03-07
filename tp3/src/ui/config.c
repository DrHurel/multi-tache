#include "../../include/ui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printUsageAndExit(char *programName) {
  printf("usage : %s [mode] [port] (ip)\n", programName);
  printf("mode = 0 for server mode\n");
  printf("mode = 1 for client mode\n");
  printf("ip is required for client mode\n");
  exit(EXIT_FAILURE);
}

UI_CONFIG loadConfig(char *argv[], int argc) {

  UI_CONFIG *config;
  if (argc < 3) {
    printUsageAndExit(argv[0]);
  }
  config = malloc(sizeof(UI_CONFIG));
  config->mode = atoi(argv[1]);
  config->port = atoi(argv[2]);
  if (config->mode == CLIENT || config->mode == CLIENT_LOOP) {
    if (argc < 4) {
      printUsageAndExit(argv[0]);
    } else {
      config->ip = malloc(
          sizeof(char) *
          16); // Assuming the IP address has a maximum length of 15 characters
      if (config->ip == NULL) {
        // Handle memory allocation failure
        exit(EXIT_FAILURE);
      }
      sscanf(argv[3], "%s", config->ip);
    }
  }

  UI_CONFIG result = *config;
  free(config);
  return result;
}

int getPort(const UI_CONFIG *self) {
  size_t n = strlen(self->ip);
  // 127.0.0.1:xxxxx
  if (n <= 9) { // min size for ipv8
    return -1;
  } else {
    if (n < 10) { // min size for ipv8 & port
      return -1;
    }
    char *res = malloc(n - 9);
    if (res == NULL) {
      return -1;
    }

    int j = 0;
    for (size_t i = 10; i < n; i++) {
      res[j] = self->ip[i];
      j++;
    }
    res[j] = '\n';

    int result = atoi(res);
    free(res);

    return result;
  }
}