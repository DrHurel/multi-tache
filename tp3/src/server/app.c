#include "server.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int handle_client(int dsClient) {
  long filezize;
  pid_t pid;
  /* The line `recvTCP(dsClient, &filezize, sizeof(long));` is receiving data
  from the client socket `dsClient` and storing it in the variable `filezize`.
  The data being received is of size `sizeof(long)` bytes. This function is
  likely a custom implementation for receiving data over a TCP connection,
  h andling any errors that may occur during the receive operation. */
  if (recvTCP(dsClient, &filezize, sizeof(long)) < 0) {
    perror("fail to receive file size:");
    close(dsClient);
    return -1;
  }

  printf("file size: %ld\n", filezize);

  /* The code snippet `long file_name_size; recvTCP(dsClient, &file_name_size,
  sizeof(long));` is receiving data from the client socket `dsClient` and
  storing it in the variable `file_name_size`. The data being received is of
  size `sizeof(long)` bytes. This likely indicates that the server is receiving
  the size of the file name that will be sent next from the client. This
  information is crucial for the server to know how much memory to allocate for
  storing the file name. */
  long file_name_size;
  if (recvTCP(dsClient, &file_name_size, sizeof(long)) < 0) {
    perror("fail to receive file name size:");
    close(dsClient);
    return -1;
  }
  printf("file name size: %ld\n", file_name_size);

  char *file_name = malloc(file_name_size + 1);
  if (file_name != NULL) {
    recvTCP(dsClient, file_name, file_name_size);
    file_name[file_name_size] = '\0';
    printf("file name: %s\n", file_name);
  } else {
    int code_err = 500;
    sendTCP(dsClient, &code_err, sizeof(int));
    printf("code erreur fail to get file_name  : %d\n", code_err);
    close(dsClient);
  }

  // start to receive file data

  int tube[2]; // tube[0] for read, tube[1] for write
  char *buf = malloc(filezize);
  if (pipe(tube) < 0) {
    perror("fail to open pipe:");
  }
  if ((pid = fork()) < 0) {
    perror("fail to fork :");
  } else if (pid > 0) { // parent write data
    close(dsClient);
  } else {
    long data_recv = 0;
    printf("start to receive data\n");
    ssize_t res = recv(dsClient, buf, 500, 0);
    data_recv += res;
    while (data_recv < filezize) {

      if (res < 0) {
        printf("data_recv: %ld\n", data_recv);
        perror("fail to receive data:");

        close(dsClient);
        return -1;
      }
      // write(tube[1], buf, res);
      res = recv(dsClient, buf + data_recv, 500, 0);
      data_recv += res;
    }

    char *file_dest = malloc(strlen("server_") + strlen(file_name) + 1);
    strcpy(file_dest, "server_");
    strcat(file_dest, file_name);
    printf("file_dest: %s\n", file_dest);
    FILE *fd = fopen(file_dest, "wb");

    if (fd == NULL) {
      perror("fail to open file");
      free(buf);
      free(file_dest);
      free(file_name);
    } else {

      fwrite(buf, 1, filezize, fd);
      fclose(fd);
      free(buf);
      free(file_dest);
      free(file_name);
      printf("file received\n");
    }

    close(dsClient);
  }

  return 1;
}

int run_server(int port, int max_concurent_connect) {
  pid_t pid;
  int ds = init_server(port, max_concurent_connect);
  if (ds < 0) {
    printf("code erreur : %d\n", ds);
    perror("fail to init server:");
    exit(EXIT_FAILURE);
  }

  while (1) {
    struct sockaddr addr;
    socklen_t addr_len;
    int dsClient = accept(ds, &addr, &addr_len);
    if (dsClient < 0) {
      perror("fail to accept :");
      continue;
    }
    if ((pid = fork()) == -1) {
      perror("fail to fork");
      close(dsClient);
      continue;
    } else if (pid > 0) { // parent processus
      close(dsClient);
    } else {
      handle_client(dsClient);
    }
  }

  return 1;
}
