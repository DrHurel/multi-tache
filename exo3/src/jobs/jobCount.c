

#include "jobs.h"
int jobCount(const Jobconfig *config) {

  int count = 0;
  for (unsigned int i = 0; i < 8; i++) {
    if ((config->code & (0b1 << i)) > 0) {
      ++count;
    }
  }
  return count;
}
