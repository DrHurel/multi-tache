/*
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
  default:
    exit(EXIT_FAILURE);
  }

  return 0;
}
*/

#include <arpa/inet.h>

#include <stdio.h>

#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main() {

  struct sockaddr_in myaddr;
  struct sockaddr_in clientaddr;
  int sockid;

  sockid = socket(AF_INET, SOCK_STREAM, 0); // create socket

  myaddr.sin_family = AF_INET;
  myaddr.sin_port = htons(8888);
  myaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  if (sockid == -1) {
    perror("socket");
  }
  printf("socket created\n");

  int len = sizeof(myaddr);
  if (bind(sockid, (struct sockaddr *)&myaddr, len) == -1) {
    perror("bind");
  }
  printf("bind done\n");

  if (listen(sockid, 10) == -1) {
    perror("listen");
  }

  printf("listening for incoming connections...\n");
  int pid;
  int new;
  static int counter = 0;
  for (;;) {
    new = accept(sockid, (struct sockaddr *)&clientaddr, (socklen_t *)&len);

    if ((pid = fork()) == -1) { // fork failed
      close(new);
      continue;
    } else if (pid > 0) { // parent process
      close(new);
      counter++;
      printf("here2\n");
      continue;
    } else if (pid == 0) { // child process
      char buf[100];

      counter++;
      printf("here 1\n");
      snprintf(buf, sizeof buf, "hi %d", counter);
      send(new, buf, strlen(buf), 0);
      close(new);
      break;
    }
  }
  printf("here3");
  close(sockid);
  return 0;
}

// Note on the fork() function:
/*
 * the fork function creates a new heavy processus, the context are different
 * but all the value of the variables initialized before the fork are the same
 * in the child processus. The fork function returns 0 in the child processus.
 * the execution of the child processus starts at the line after the fork. We
 * fork than we evaluate the pid to know if we are in the child or the parent
 * processus.
 */