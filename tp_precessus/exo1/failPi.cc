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

void askCoin(Locker_msg *coin, Header_rcv *header, int f_id) {
  // send request
  if (msgsnd(f_id, coin, sizeof(Locker_msg), 0) < 0) {
    perror("fail to ask");
  }

  // wait for validation and string size
  if (msgrcv(f_id, header, sizeof(Header_rcv), coin->pid, 0) < 0) {
    perror("fail get coin:");
  }
}

void freeCoin();

int action(int id, int f_id) {

  printf("%d blabla pas critique : \n", id);
  Locker_msg coin;
  coin.etiq = 1;
  coin.pid = getpid();
  printf("pid %ld\n", coin.pid);
  auto header = Header_rcv{getpid(), 0};
  askCoin(&coin, &header, f_id);

  printf("%d blabla critique\n", id);
  sleep(2); // simulation d'un action
  auto temp = (char *)malloc(sizeof(char) * header.size);
  if (temp == NULL) {
    perror("fail malloc");
  }
  auto buf = Buf_rev{getpid(), temp};

  if (msgrcv(f_id, &buf, sizeof(char) * header.size + sizeof(long), coin.pid,
             0) < 0) {
    perror("fail step 3:");
  }
  printf("%d toujours critique : %s\n", id, buf.data);

  coin.etiq = coin.pid;
  auto r = msgsnd(f_id, &coin, sizeof(coin.pid), 0);
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
  key_t key = ftok(argv[1], atoi(argv[2]));
  auto f_id = msgget(key, O_RDWR);
  if (f_id == -1) {
    perror("msgget failed");
    return -1;
  }
  if (f_id < 0) {
    perror("fail to load");
  }
  for (int i = 0; i <= n; ++i) {
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
