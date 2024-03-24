#ifndef CALCUL_H
#define CALCUL_H

#include <unistd.h>

// Cette fonction effectue un calcul d'une durée environ 3 * sec secondes.
void calcul(int sec) { sleep(sec * 3); }
#endif