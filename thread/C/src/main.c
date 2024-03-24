#include <pthread.h>
#include <stdio.h>
#include <threads.h>

void *func(void *arg) {
  char *str = (char *)arg;

  for (int i = 0; i < 10; i++) {
    printf("%s\n", str);
  }
  printf("Hello from thread!\n");
  return NULL;
}

typedef struct {
  int a;
  const char *b;
} my_struct;

void *func2(void *arg) {
  const my_struct *str = (my_struct *)arg;

  for (int i = 0; i < 10; i++) {
    printf("%d\n", str->a);
    printf("%s\n", str->b);
  }
  printf("Hello from thread 2!\n");
  return NULL;
}

int main(int argc, char const *argv[]) {
  printf("Hello World!\n");

  pthread_t thread = 0;
  pthread_t thread2 = 0;

  pthread_create(&thread, NULL, func, "Hello from thread!");

  printf("Hello World 2!\n");

  my_struct str = {10, "Hello from thread 2!"};
  pthread_create(&thread, NULL, func2, &str);

  pthread_create(&thread2, NULL, func, "Hello  thread!");

  pthread_join(thread2, NULL);
  pthread_join(thread, NULL);

  return 0;
}
