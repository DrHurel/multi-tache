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

void lock(Locker_msg *coin, int f_id) {
  // wait for a msg
  if (msgrcv(f_id, coin, sizeof(Locker_msg), 0, 0) < 0) {
    perror("fail to lock");
  }
  coin->etiq = coin->pid;
  auto header = Header_rcv{coin->etiq, 10};

  // send confirmation
  if (msgsnd(f_id, &header, sizeof(header), 0) < 0) {
    perror("fail validate lock:");
  }
}

void critical_action(Locker_msg *coin, int f_id) {
  auto data = (char *)"123456789";
  auto buf = Buf_rev{coin->pid, data};

  if (msgsnd(f_id, &buf, sizeof(Buf_rev), 0) < 0) {
    perror("fail step 3:");
  }
}

void unlock(Locker_msg *coin, int f_id) {
  printf("wait to unlock\n");
  if (msgrcv(f_id, coin, sizeof(Locker_msg), coin->pid, 0) < 0) {
    perror("fail step 4:");
  }
  coin->etiq = 1;
}

int main(const int argc, char *argv[]) {

  if (argc != 4) {
    std::cout << "Nbre d'args invalide, utilisation :" << std::endl;
    std::cout << argv[0] << " <fichier-pour-cle-ipc> <entier-pour-cle-ipc> <n>"
              << std::endl;
    exit(EXIT_FAILURE);
  }

  key_t key = ftok(argv[1], atoi(argv[2]));
  auto f_id = msgget(key, IPC_CREAT | 0666);
  auto coin = Locker_msg{1};
  while (true) {
    printf("wait to lock\n");
    lock(&coin, f_id);

    printf("wait\n");
    critical_action(&coin, f_id);
    unlock(&coin, f_id);
  }

  return 0;
}