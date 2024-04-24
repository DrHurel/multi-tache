#include <cstdlib>
#include <fcntl.h>
#include <iostream>
#include <stdio.h> //perror
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
int main(const int argc, char *argv[]) {

  if (argc != 3) {
    std::cout << "Nbre d'args invalide, utilisation :" << std::endl;
    std::cout << argv[0] << " fichier-pour-cle-ipc entier-pour-cle-ipc"
              << std::endl;
    exit(0);
  }

  key_t cle = ftok(argv[1], atoi(argv[2]));

  int uneClef = msgget(cle, IPC_CREAT | 0666);
  struct {
    long etiq;
    char mot[12];
  } vmsgsnd;
  for (int i = 0; i < 11; i++) {
    vmsgsnd.mot[i] = 'a' + (char)i;
  }
  vmsgsnd.mot[11] = '\0';
  vmsgsnd.etiq = getpid();

  pid_t pid;
  int status;
  int mp = msgsnd(uneClef, &vmsgsnd, sizeof(vmsgsnd.mot), 0);

  for (int i = 0; i < 100; i++) {
    if ((pid = fork()) == -1) {
      exit(EXIT_FAILURE);
    } else if (pid == 0) {
      // action
      sleep(rand() % 2);
      key_t temp = msgget(cle, O_RDWR);
      struct {
        long etiq;
        char mot[12];
      } vmsg;

      msgrcv(temp, &vmsg, sizeof(vmsg.mot), getppid(), 0);

      printf("m %i %c\n", vmsg.mot[0] == i, vmsg.mot[1]);
      break;
    } else {
      vmsgsnd.mot[0] = (char)i;
      continue;
    }
  }

  while (true) {

    pid_t done = wait(&status);
    if (done == -1) {
      if (errno == ECHILD)
        break; // no more child processes
    } else {
      if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
        std::cerr << "pid " << done << " failed" << std::endl;
        exit(1);
      }
    }
  }

  if (pid != 0) {
    printf("fini\n");
  }
}