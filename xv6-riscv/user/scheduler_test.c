/*Adut Beny
  oae862
  11349460

  Mustafa Saqib
  xvp605
  11330309
*/




#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define PROCS 40

int itoa(int value, char *str, int base) {
    char *start = str;
    if (value < 0) {
        *str++ = '-';
        value = -value;
    }
    char buf[20]; // Enough for any 32-bit integer
    char *ptr = buf + sizeof(buf);
    *--ptr = '\0';
    do {
        *--ptr = "0123456789"[value % base];
        value /= base;
    } while (value);
    while (*ptr)
        *str++ = *ptr++;
    return str - start; // Return length of the string
}


/* from FreeBSD. */
int
do_rand(unsigned long *ctx)
{
/*
 * Compute x = (7^5 * x) mod (2^31 - 1)
 * without overflowing 31 bits:
 *      (2^31 - 1) = 127773 * (7^5) + 2836
 * From "Random number generators: good ones are hard to find",
 * Park and Miller, Communications of the ACM, vol. 31, no. 10,
 * October 1988, p. 1195.
 */
    long hi, lo, x;

    /* Transform to [1, 0x7ffffffe] range. */
    x = (*ctx % 0x7ffffffe) + 1;
    hi = x / 127773;
    lo = x % 127773;
    x = 16807 * lo - 2836 * hi;
    if (x < 0)
        x += 0x7fffffff;
    /* Transform to [0, 0x7ffffffd] range. */
    x--;
    *ctx = x;
    return (x);
}

unsigned long rand_next = 1;

int
rand(void)
{
    return (do_rand(&rand_next));
}
 

int square (int n ){
	return (n*n);

}


void child(int group_id){
	int i;

	int sleep_time = rand() % 100;
	int squaring = rand() % 5000 + 1000;

	sleep(sleep_time);

	for(i =0; i<squaring ; i++){
		square(i);
		//printf("w");
	}
	
//	printf("group %d , PID %d finished work \n", group_id, getpid());
	exit(1);
}


/*
int main(){

	//int i;
	int j;


//	int groups = 4;
	int procs = 4;

	for( j=0; j<procs ; j++){
		int pid = fork();
		if(pid < 0){
			printf("fork failed \n");
			exit(1);
		}else if(pid ==0){
			child(j);
		}
	}



	for(j =0; j<procs ; j++){
		int status;
		int child_pid = wait(&status);
		
		if(status==1){
			printf("Group ID %d is done (Child PID: %d)\n", group_id - n_children + i, child_pid);
		}	
		wait(0);
	}

	printf("DONE ! \n");
	return 0;



}


*/

int main() {
    int procs = PROCS; // Number of child processes

    for (int j = 0; j < procs; j++) {
        int pid = fork();
        if (pid < 0) {
            printf("Fork failed\n");
            exit(1);
        } else if (pid == 0) {
            // Child process
            child(j);
        }
    }

    // Parent process waits for all children
    for (int j = 0; j < procs; j++) {
        int status;
        int child_pid = wait(&status); // Wait for one child process to finish

        // Directly check the status
        if (status == 1) { // If status equals 1 (child exited with 1)
            printf("Group ID %d is done (Child PID: %d)\n", j, child_pid);
        } else {
            printf("Child PID %d exited with unexpected status: %d\n", child_pid, status);
        }
    }

    printf("DONE!\n");
    return 0;
}
