#ifndef SERVER_H
#define SERVER_H

#include <netinet/in.h>

int init_server(int port, int max_concurent_connect);
int run_server(int port, int max_concurent_connect);

#endif // SERVER_H
