/* Adut Beny
   oae862
   11349460

   Kiran Epp
   kee042
   11233008
*/

#include "kernel/types.h"
#include "user/user.h"
#include "kernel/syscall.h"

int main(int argc, char** argv){

	if (argc < 3){
		printf("Need at least 3 arguments");
		return(-1);
	}

	int mask = atoi(argv[1]);
	trace(mask);
	exec(argv[2], &argv[2]);
	exit(0);

}

