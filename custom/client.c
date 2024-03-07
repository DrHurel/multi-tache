#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>

/**
 *
 */

int main(int argc, char **argv)
{

    if (argc != 4)
    {
        printf("utilisation : %s ip_serveur port_serveur port_client\n", argv[0]);
        exit(1);
    }

    int sckt = socket(PF_INET, SOCK_DGRAM, 0);

    if (sckt == -1)
    {
        perror("Client : error creation socket :");
        exit(1); // je choisis ici d'arrêter le programme car le reste
                 // dépendent de la réussite de la création de la socket.
    }

    printf("Client : creation de la socket réussie \n");

    struct sockaddr_in ad;
    ad.sin_family = AF_INET;
    ad.sin_addr.s_addr = INADDR_ANY;
    ad.sin_port = htons((short)atoi(argv[3]));
    int bd = bind(ds, (struct sockaddr *)&ad, sizeof(ad));

    if (bd == -1)
    {
        perror("Client : pb creation socket :");
        exit(1); // je choisis ici d'arrêter le programme car le reste
                 // dépendent de la réussite de la création de la socket.
    }

    /* Etape 3 : Désigner la socket du serveur */
    struct sockaddr_in ads;
    ads.sin_family = AF_INET;
    ads.sin_addr.s_addr = inet_addr(argv[1]);
    ads.sin_port = htons((short)atoi(argv[2]));

    return 0;
}
