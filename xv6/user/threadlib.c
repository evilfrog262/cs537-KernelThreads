#include "types.h"
#include "stat.h"
#include "user.h"
#include "param.h"
#include "x86.h"
#include "threadlib.h"

//thread_join();

/*void lock_acquire(lock_t *lock) {
  while (xchg(lock->value, 1) == 1) {
    ;
  }
}

void lock_release(lock_t *lock) {
  xchg(lock->value, 0);
}

void lock_init(lock_t *lock) {
  xchg(lock->value, 0);
}

typedef struct __cond_t {
  int state;
  lock_t cvlock;
} cond_t;


int thread_create(void (*start_routine)(void*), void *arg){
   void* stack = malloc(10);
   stack = NULL;
   return 0;
}*/
