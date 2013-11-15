#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"

int main(int argc, char** argv) {
  // lock test
  lock_t *lock = malloc(sizeof(lock_t));
  lock_init(lock);
  lock_acquire(lock);
  int a = 1;
  lock_release(lock);
  printf(1, "a: %d\n", a);
  
  // clone test
  void *stack = malloc(4096);
  int *arg;
  arg = &a;
  int pid =  clone(arg, stack);
  printf(1, "pid: %d\n", pid);

  exit();
}
