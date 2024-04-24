#include <iostream>
#include <stdio.h> //perror
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>

// pour supprimer la file ayant la clé passée en parametre.

// une autre solution est d'utiliser la commande ipcrm -q <id_file>
// après avoir obtenue l'id de la file via la commande ipcs.

int main(int argc, char *argv[]) {

  if (argc != 3) {
    std::cout << "Nbre d'args invalide, utilisation :" << std::endl;
    std::cout << argv[0] << " fichier-pour-cle-ipc entier-pour-cle-ipc"
              << std::endl;
    exit(0);
  }

  key_t cle = ftok(argv[1], atoi(argv[2]));

  if (cle == -1) {
    perror("Erreur ftok : ");
    exit(2);
  }

  std::cout << "ftok ok" << std::endl;

  int msgid = msgget(cle, 0666);
  if (msgid == -1) {
    perror("erreur msgget : ");
    exit(2);
  }

  std::cout << "msgget ok" << std::endl;

  if (msgctl(msgid, IPC_RMID, nullptr) == -1)
    perror("erreur suppression file de message :");

  std::cout << "suppression file ok" << std::endl;

  return 0;
}
