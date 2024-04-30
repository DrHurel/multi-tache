#ifndef SYNC_H
#define SYNC_H

void waitFor(int semid, short target, short zone);

void signalZone(int semid, short target, short zone);

#endif // SYNC_H