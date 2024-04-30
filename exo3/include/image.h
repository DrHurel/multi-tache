#ifndef IMAGE_H
#define IMAGE_H

typedef struct {
  int width;
  int height;
  char *ref;
} Image;

void createImage(const char *ref, Image *res, int cle);

#endif // IMAGE_H