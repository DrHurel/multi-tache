#include <iostream>
#include <stdio.h> //perror
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
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

  int f_id = msgget(cle, IPC_CREAT | 0666); // créé a file si pas existante

  if (f_id == -1) {
    perror("Erreur msgget : ");
    exit(3);
  }

  cout << "creation de msgget" << endl;

  struct sMsg vMsgP;
  vMsgP.etiq = (long)1;
  vMsgP.pid = 0;
  int lui;

  int pastouche = 0;

  // attente de demande d'acces
  while (1) {
    printf("attente un utilisateur pour entré en section critique\n");
    struct sMsg vMsg;
    int ret = msgrcv(f_id, &vMsg, sizeof(vMsg), (long)1, 0); // recoi étiquette
                                                             // 1

    if (ret == -1) {
      perror("Erreur msgrcv : ");
      exit(4);
    }

    printf("            pasé            \n");

    printf("il rendre en section kritik : PID : %i\n", vMsg.pid);
    printf("envoi de validation d'entré\n");
    struct sMsg vMsgr;
    vMsgr.etiq = (long)vMsg.pid;
    vMsgr.pid = getpid();
    int res = msgsnd(f_id, &vMsgr, sizeof(vMsgr), 0); // envoi autoris

    if (res == -1) {
      perror("Erreur msgsnd : ");
      exit(5);
    }

    printf("attente de modification\n");

    int ret2 =
        msgrcv(f_id, &vMsg, sizeof(vMsg), (long)getpid(), 0); // reçoit modif

    if (ret2 == -1) {
      perror("Erreur msgrcv : ");
      exit(6);
    }

    printf("modification effectuer\n");
    struct sMsg vMsgr2;
    vMsgr2.etiq = (long)vMsg.pid;
    vMsgr2.pid = 1;
    printf("envoi de notification de fin de section kritik\n");
    int res2 =
        msgsnd(f_id, &vMsgr2, sizeof(vMsgr2), 0); // envoi bien fini pour toi

    if (res2 == -1) {
      perror("Erreur msgsnd : ");
      exit(7);
    }
  }
}