// ui.h

#ifndef UI_H
#define UI_H

// Include necessary libraries

typedef enum { SERVER, CLIENT } APP_MODE;

typedef struct {
  int port;
  char *ip;
  APP_MODE mode;

} UI_CONFIG;

UI_CONFIG loadConfig(char *argv[], int argc);
int getPort(const UI_CONFIG *self);

#endif // UI_H