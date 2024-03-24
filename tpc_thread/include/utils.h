#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
int sendTCP(int sock, const void *msg, long sizeMsg);

int recvTCP(int sock, void *msg, long sizeMsg);
long file_size(FILE *fd, const char *path);
int writeFile(const int tube[2], FILE *fd);
#endif // UTILS_H
