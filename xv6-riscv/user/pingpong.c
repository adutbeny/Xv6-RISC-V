/* Adut Beny
   oae862
   11349460

   Kiran Epp
   kee042
   11233008
*/

#include "kernel/types.h"
#include "user/user.h"

int main(){
	int p[2];
	int q[2];
	char byte;
	pipe(p);
	pipe(q);

	if (fork() == 0){
		int byteChild = read(p[0], &byte, 1);
		if (byteChild > 0){
			printf("%d: received ping\n", getpid());
		}
		close(p[0]);
		write(q[1], &byte, 1);
		close(q[1]);
	}
	else{
		write(p[1], &byte, 1);
		close(p[1]);
		int byteParent = read(q[0], &byte, 1);
		if (byteParent > 0){
			printf("%d: received pong\n", getpid());
		}
		close(q[0]);


	}
	exit(0);



}
