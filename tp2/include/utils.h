#ifndef UTILS_H
#define UTILS_H

int sendTCP(int sock, const void *msg, int sizeMsg);

int recvTCP(int sock, void *msg, int sizeMsg);

#endif // UTILS_H
