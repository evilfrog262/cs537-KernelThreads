#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"

#include "fcntl.h"
#include "x86.h"

#undef NULL
#define NULL ((void*)0)

#define PGSIZE (4096)

int ppid;
volatile uint newfd = 0;

#define assert(x) if (x) {} else { \
   printf(1, "%s: %d ", __FILE__, __LINE__); \
   printf(1, "assert failed (%s)\n", # x); \
   printf(1, "TEST FAILED\n"); \
   kill(ppid); \
   exit(); \
}

void worker(void *arg_ptr);

int
main(int argc, char *argv[])
{
   ppid = getpid();
   void *stack = malloc(PGSIZE*2);
   assert(stack != NULL);
   if((uint)stack % PGSIZE)
     stack = stack + (4096 - (uint)stack % PGSIZE);

   int fd = open("tmp", O_WRONLY|O_CREATE);
   assert(fd == 3);
   int clone_pid = clone(worker, 0, stack);
   assert(clone_pid > 0);
   //printf(1, "clone pid: %d\n", clone_pid);
   while(!newfd);
   assert(write(newfd, "goodbye\n", 8) == -1);
   printf(1, "TEST PASSED\n");
   exit();
}

void
worker(void *arg_ptr) {
   printf(1, "in function\n");
   assert(write(3, "hello\n", 6) == 6);
   printf(1, "passed assertion\n");
   xchg(&newfd, open("tmp2", O_WRONLY|O_CREATE));
   printf(1, "passed xchg\n");
   exit();
}

//void printstuff(void* str);

/*int main(int argc, char** argv) {
  // lock test
  lock_t *lock = malloc(sizeof(lock_t));
  lock_init(lock);
  lock_acquire(lock);
  int a = 1;
  lock_release(lock);
  printf(1, "a: %d\n", a);
  
  // clone test
  void *stack = malloc(4096);
  char* word = "word";
  char** arg;
  arg = &word;
  void (*fcn)(void*) = &printstuff;
  int pid =  clone(fcn, word, stack);
  printf(1, "pid: %d\n", pid);

  exit();
}

void printstuff(void* str) {
  printf(1, "%s\n", (char*)str);
}*/
