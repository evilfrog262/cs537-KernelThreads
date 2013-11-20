/* check that address space size is updated in threads */
#include "types.h"
#include "user.h"

#undef NULL
#define NULL ((void*)0)

#define PGSIZE (4096)

int ppid;
int global = 0;
unsigned int size = 0;
lock_t lock, lock2;
int num_threads = 30;


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
   printf(1, "started test\n");
   ppid = getpid();

   int arg = 101;
   void *arg_ptr = &arg;

   lock_init(&lock);
   lock_init(&lock2);

   printf(1, "initialized locks: \n%d\n%d\n", &lock, &lock2);

   lock_acquire(&lock);

   printf(1, "acquired lock\n");
   lock_acquire(&lock2);

   printf(1, "acquired lock2\n");

   int i;
   for (i = 0; i < num_threads; i++) {
      int thread_pid = thread_create(worker, arg_ptr);
      assert(thread_pid > 0);
   }
   printf(1, "passed first loop\n");

   size = (unsigned int)sbrk(0);

   while (global < num_threads) {
      lock_release(&lock);
      sleep(100);
      lock_acquire(&lock);
   }
   printf(1, "passed 2nd loop\n");

   global = 0;
   sbrk(10000);
   size = (unsigned int)sbrk(0);
   lock_release(&lock);

   while (global < num_threads) {
      lock_release(&lock2);
      sleep(100);
      lock_acquire(&lock2);
   }
   lock_release(&lock2);
   printf(1, "passed 3rd loop\n");

   for (i = 0; i < num_threads; i++) {
      int join_pid = thread_join();
      assert(join_pid > 0);
   }

   printf(1, "TEST PASSED\n");
   exit();
}

void
worker(void *arg_ptr) {
   lock_acquire(&lock);
   assert((unsigned int)sbrk(0) == size);
   global++;
   lock_release(&lock);

   lock_acquire(&lock2);
   assert((unsigned int)sbrk(0) == size);
   global++;
   lock_release(&lock2);

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
