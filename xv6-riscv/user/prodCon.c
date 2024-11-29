/* Adut Beny
   oae862
   11349460

   Kiran Epp
   kee042
   11233008
*/

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

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


extern void thread_init(void);
extern void thread_schedule(void);
extern void thread_create(void(*tmain)());
extern void thread_yield(void);
extern int mtx_create(int);
extern int mtx_lock(int);
extern int mtx_unlock(int);
int Mutex;
int full;
int empty = 10;
int buffer[10];
int location = 0;


/* CMPT 332 GROUP 12 Change, Fall 2024 */
struct mutexlock{
        int locked;
        int lockID;
};

extern struct mutexlock mutexArray[10];
extern int used;

void producer(void){
	int i;
	printf("Producer\n");
	mtx_lock(Mutex);
	for (;;){
		while (empty > 0) {
			printf("producer producing\n");
			buffer[location] = Mutex;
			empty--;
			location++;
			printf("Array contents: ");
			printf("[");
                	for (i = 0; i < location; i++){
               			printf("%d ", buffer[i]);
			}
			printf("]\n");
			mtx_unlock(Mutex);
			thread_schedule();
		}
	}


 }

void consumer(void){
	int i;
	printf("Consumer\n");
	mtx_lock(Mutex);
	for(;;){
		while (empty > 0){
			printf("consumer consuming\n");
			buffer[location] = 0;
			empty++;
			location--;
			printf("Array contents: ");
			printf("[");
			for (i = 0; i < location; i++){
				printf("%d ", buffer[i]);
			}
			printf("]\n");
			mtx_unlock(Mutex);
			thread_schedule();
		}
	}
	
}

int cmain(){
	thread_init();
	Mutex = mtx_create(0);
	printf("Mutex created!\n");
	thread_create(producer);
	thread_create(consumer);
	thread_schedule();
	printf("Everything was created!\n");
	exit(0);

}
