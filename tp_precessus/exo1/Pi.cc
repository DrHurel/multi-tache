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

using Header_rcv = struct {
  long etiq;
  int size;
};

using Locker_msg = struct {
  long etiq;
  long pid;
};

using Buf_rev = struct {
  long etiq;
  char *data;
};

int action(int id, key_t f_id) {
  auto key = msgget(f_id, O_RDWR);
  if (key == -1) {
    perror("msgget failed");
    return -1;
  }
  printf("%d blabla pas critique : \n", id);
  Locker_msg lock;
  lock.etiq = 0;
  lock.pid = getpid();
  auto r = msgsnd(key, &lock, sizeof(lock) - sizeof(long), 0);
  if (r < 0) {
    perror("fail to ask");
  }
  auto header = Header_rcv{getpid()};
  msgrcv(key, &header, sizeof(int), lock.pid, 0);
  printf("%d blabla critique\n", id);
  sleep(2);
  auto buf = Buf_rev{getpid(), (char *)malloc(sizeof(char) * header.size)};

  msgrcv(key, &buf, sizeof(char) * header.size, lock.pid, 0);
  printf("%d toujours critique : %s\n", id, buf.data);

  lock.etiq = lock.pid;
  r = msgsnd(key, &lock, sizeof(lock.pid), 0);
  if (r < 0) {
    perror("fail to unlock");
  }

  printf("%d plus critique\n", id);
  free(buf.data);
  sleep(1);
  printf("%d fin pas critique\n", id);

  return 1;
}

int main(const int argc, char *argv[]) {

  if (argc != 4) {
    std::cout << "Nbre d'args invalide, utilisation :" << std::endl;
    std::cout << argv[0] << " <fichier-pour-cle-ipc> <entier-pour-cle-ipc> <n>"
              << std::endl;
    exit(EXIT_FAILURE);
  }

  int n = atoi(argv[3]);
  pid_t pid;
  key_t f_id = ftok(argv[1], atoi(argv[2]));
  if (f_id < 0) {
    perror("fail to load");
  }
  for (int i = 0; i < n; ++i) {
    if ((pid = fork()) == -1) {
      perror("fait to start : ");
    } else if (pid == 0) {
      return action(i, f_id);
    } else {
      continue;
    }
  }

  int status;
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

  return 0;
}
