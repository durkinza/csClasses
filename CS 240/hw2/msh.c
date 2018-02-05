/* My Shell
 * Author: Zane Durkin <durk7832@vandals.uidaho.edu>
 * CS 240
 */

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

void main(){
	pid_r childpid;
	/*if((childpid = fork()) ==1){
		printf("\n Shell's PID is %ld\n", (long)getppid());
	}*/
	if((childpid = fork()) == 0){ //child will start at fork()
		printf(stderr, "\nI am child, ID = %ld\n", (long)getpid());
	}else if(childpid > 0){
		printf(stderr, "\nI am parent, ID = %ld\n", (long)getpid());	
	}else{
		// if childpid == -1
		printf(stderr, "\nError in fork");
	}

}
