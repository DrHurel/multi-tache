#include <cstddef>
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

int main(const int argc, char *argv[]) {

  if (argc != 3) {
    std::cout << "Nbre d'args invalide, utilisation :" << std::endl;
    std::cout << argv[0] << " <fichier-pour-cle-ipc> <entier-pour-cle-ipc> <n>"
              << std::endl;
    exit(EXIT_FAILURE);
  }

  key_t f_id = ftok(argv[1], atoi(argv[2]));
  auto key = msgget(f_id, IPC_CREAT | 0666);
  auto lock = Locker_msg{0};
  while (true) {
    printf("wait to lock\n");
    msgrcv(key, &lock, sizeof(long), 0, 0);

    lock.etiq = lock.pid;
    auto header = Header_rcv{lock.etiq, 10};
    msgsnd(key, &header, sizeof(int), 0);
    auto data = (char *)"123456789";
    auto buf = Buf_rev{lock.pid, data};
    printf("wait to unlock\n");
    msgsnd(key, &buf, sizeof(char) * 10, 0);
    msgrcv(key, &lock, sizeof(int), lock.pid, 0);
    lock.etiq = 0;
  }

  return 0;
}