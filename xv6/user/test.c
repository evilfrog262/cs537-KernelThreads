#include "types.h"
#include "user.h"

#undef NULL
#define NULL ((void*)0)

#define PGSIZE (4096)

int ppid;
volatile int global;

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

   int i, thread_pid, join_pid;
   for(i = 0; i < 2000; i++) {
      global = 1;
      thread_pid = thread_create(worker, 0);
      assert(thread_pid > 0);
      join_pid = thread_join();
	  printf(1, "returned from JOIN\n");
      assert(join_pid == thread_pid);
      assert(global == 5);
      assert((uint)sbrk(0) < (150 * 4096) && "shouldn't even come close");
   }

   printf(1, "TEST PASSED\n");
   exit();
}

void
worker(void *arg_ptr) {
   assert(global == 1);
   global+=4;
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
