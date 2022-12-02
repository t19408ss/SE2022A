#include "lock.h"
std::atomic<int> Counter(0);

void *worker(void *arg) {
  for (uint i = 0; i < NbLoop; i++) {
    while(true) {
      auto exp = Counter.load();
      bool ok = Counter.compare_exchange_strong(exp, exp + 1);
      if (ok == true) break;
    }
  }
  return NULL;
}

int
main(void)
{
  pthread_t thread[NbThread];
  if ((pthread_mutex_init(&Lock, NULL)) == -1) ERR;

  for (uint i = 0; i < NbThread; i++) {
    pthread_create(&thread[i], NULL, worker, NULL);
  }
  for (uint i = 0; i < NbThread; i++) {
    pthread_join(thread[i], NULL);
  }
  printf("Counter: %u (Ref. %u)\n", Counter.load(), NbThread * NbLoop);

  return 0;
}
