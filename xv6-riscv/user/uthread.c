/* Adut Beny
 * oae862
 * 11349460
 * 
 * Kiran Epp
 * kee042
 * 11233008
 */

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

/* Possible states of a thread: */
#define FREE        0x0
#define RUNNING     0x1
#define RUNNABLE    0x2

#define STACK_SIZE  8192
#define MAX_THREAD  4

/* CMPT 332 GROUP 12 Change, Fall 2024 */
struct registers{
	uint64 ra;
	uint64 sp;
	uint64 s1;
	uint64 s2;
	uint64 s3;
	uint64 s4;
	uint64 s5;
	uint64 s6;
	uint64 s7;
	uint64 s8;
	uint64 s9;
	uint64 s10;
	uint64 s11;
};



struct thread {
  /* CMPT 332 GROUP 12 Change, Fall 2024 */
  struct registers threadRegisters;
  char       stack[STACK_SIZE]; /* the thread's stack */
  int        state;             /* FREE, RUNNING, RUNNABLE */
};

/* CMPT 332 GROUP 12 Change, Fall 2024 */
struct mutexlock{
        int locked;
        int lockID;
};
struct mutexlock mutexArray[10];
int used = 0;

struct thread all_thread[MAX_THREAD];
struct thread *current_thread;
/* CMPT332 GROUP 12 Change, Fall 2024 */
extern void thread_switch(struct registers*, struct registers*);


void thread_init(void)
{
  // main() is thread 0, which will make the first invocation to
  // thread_schedule().  it needs a stack so that the first thread_switch() can
  // save thread 0's state.  thread_schedule() won't run the main thread ever
  // again, because its state is set to RUNNING, and thread_schedule() selects
  // a RUNNABLE thread.
  current_thread = &all_thread[0];
  current_thread->state = RUNNING;
}

void 
thread_schedule(void)
{
  struct thread *t, *next_thread;

  /* Find another runnable thread. */
  next_thread = 0;
  t = current_thread + 1;
  for(int i = 0; i < MAX_THREAD; i++){
    if(t >= all_thread + MAX_THREAD)
      t = all_thread;
    if(t->state == RUNNABLE) {
      next_thread = t;
      break;
    }
    t = t + 1;
  }

  if (next_thread == 0) {
    printf("thread_schedule: no runnable threads\n");
    exit(-1);
  }

  if (current_thread != next_thread) {         /* switch threads?  */
    next_thread->state = RUNNING;
    t = current_thread;
    current_thread = next_thread;
     /* CMPT 332 GROUP 12 Change, Fall 2024 */
    thread_switch(&t->threadRegisters, &current_thread->threadRegisters);
  } else
    next_thread = 0;
}


void 
thread_create(void (*tmain)())
{
  struct thread *t;
  t = malloc(sizeof(struct thread));
  if (t == 0){
	exit(1);
  }

  for (t = all_thread; t < all_thread + MAX_THREAD; t++) {
    if (t->state == FREE) break;
  }
  t->state = RUNNABLE;
  /* CMPT 332 GROUP 12 Change, Fall 2024 */
  if (t >= all_thread + MAX_THREAD){
	return;
  }

  memset(&t->threadRegisters, 0, sizeof t->threadRegisters);

  t->threadRegisters.ra = (uint64)tmain;
  t->threadRegisters.sp = (uint64)(STACK_SIZE + t->stack);

}

void 
thread_yield(void)
{
  current_thread->state = RUNNABLE;
  thread_schedule();
}

/* CMPT 332 GROUP 12 Change, Fall 2024 */
int mtx_create(int locked){
        if (used > 9){
                printf("Max amount of mutexes!");
                return -1;
        }
        mutexArray[used].lockID = used;
        mutexArray[used].locked = locked;
        used++;
        return mutexArray[used-1].lockID;
}

int mtx_lock(int lock_id){
        if (lock_id > 9){
                printf("NON valid lock ID!");
                return -1;
        }
        while (mutexArray[lock_id].locked == 1) {
                thread_yield();
        }
        mutexArray[lock_id].locked = 1;
        return 0;

}

int mtx_unlock(int lock_id){
       if (lock_id > 9){
                printf("NON valid lock ID!");
                return -1;
        }
        mutexArray[lock_id].locked = 0;
        thread_yield();
        return 0;
}


volatile int a_started, b_started, c_started;
volatile int a_n, b_n, c_n;

void 
thread_a(void)
{
  int i;
  printf("thread_a started\n");
  a_started = 1;
  while(b_started == 0 || c_started == 0)
    thread_yield();
  
  for (i = 0; i < 100; i++) {
    printf("thread_a %d\n", i);
    a_n += 1;
    thread_yield();
  }
  printf("thread_a: exit after %d\n", a_n);

  current_thread->state = FREE;
  thread_schedule();
}

void 
thread_b(void)
{
  int i;
  printf("thread_b started\n");
  b_started = 1;
  while(a_started == 0 || c_started == 0)
    thread_yield();
  
  for (i = 0; i < 100; i++) {
    printf("thread_b %d\n", i);
    b_n += 1;
    thread_yield();
  }
  printf("thread_b: exit after %d\n", b_n);

  current_thread->state = FREE;
  thread_schedule();
}

void 
thread_c(void)
{
  int i;
  printf("thread_c started\n");
  c_started = 1;
  while(a_started == 0 || b_started == 0)
    thread_yield();
  
  for (i = 0; i < 100; i++) {
    printf("thread_c %d\n", i);
    c_n += 1;
    thread_yield();
  }
  printf("thread_c: exit after %d\n", c_n);

  current_thread->state = FREE;
  thread_schedule();
}

int 
main(int argc, char *argv[]) 
{
  a_started = b_started = c_started = 0;
  a_n = b_n = c_n = 0;
  thread_init();
  thread_create(thread_a);
  thread_create(thread_b);
  thread_create(thread_c);
  thread_schedule();
  exit(0);
}
