#include <fcntl.h>
#include <iostream>
#include <stdio.h> //perror
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
using namespace std;

struct sMsg {
  long etiq;
  int pid;
};

int main(int argc, char *argv[]) {
  if (argc != 3) {
    cout << "Nbre d'args invalide, utilisation :" << endl;
    cout << argv[0] << " fichier-pour-cle-ipc entier-pour-cle-ipc" << endl;
    exit(0);
  }

  key_t cle = ftok(argv[1], atoi(argv[2]));

  if (cle == -1) {
    perror("Erreur ftok : ");
    exit(2);
  }

  cout << "ftok ok" << endl;

  cout << "creation de msgget" << endl;

  int f_id2 = msgget(cle, IPC_CREAT | 0666);
  if (f_id2 == -1) {
    perror("Erreur msgget f_id2: ");
    exit(3);
  }

  struct sMsg vMsg;
  vMsg.etiq = 1; // pas 0 car sinon on rentre dans la boucle infinie
  vMsg.pid = getpid();
  int res = msgsnd(f_id2, &vMsg, sizeof(vMsg), 0); // envoi étiquete 1
  if (res == -1) {
    perror("Erreur msgsnd res: ");
    exit(4);
  }
  printf("j'attend la validation PID : %i\n", getpid());
  struct sMsg vMsgr;
  int ret =
      msgrcv(f_id2, &vMsgr, sizeof(vMsgr), (long)getpid(), 0); // validation ?
  if (ret == -1) {
    perror("Erreur msgrcv ret: ");
    exit(5);
  }

  printf("je suis validé\n");
  struct sMsg vMsg2;
  vMsg2.etiq = vMsgr.pid;
  vMsg2.pid = getpid();
  int res2 = msgsnd(f_id2, &vMsg2, sizeof(vMsg2), 0); // envoi modif
  if (res2 == -1) {
    perror("Erreur msgsnd res2: ");
    exit(6);
  }
  printf("modification envoyer\n");

  struct sMsg vMsgr2;
  int ret2 = msgrcv(f_id2, &vMsgr2, sizeof(vMsgr2), (long)getpid(), 0); // fini
                                                                        // ?
  if (ret2 == -1) {
    perror("Erreur msgrcv ret2: ");
    exit(7);
  }
  printf("j'ai bien fini\n");
  return EXIT_SUCCESS;
}