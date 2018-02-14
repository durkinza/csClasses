/* My Shell
 * Author: Zane Durkin <durk7832@vandals.uidaho.edu>
 * CS 240
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_COMMAND 255

void prompt(char *com[MAX_COMMAND][MAX_COMMAND]);// output '?:' and wait for response
int main(){
	pid_t childpid;
	char * command[MAX_COMMAND];
	int com_len, child_ret_val;
	while(1){
		prompt(&command);
		//printf("commmand is %s\n", com ); 
		if( strcmp( command[0], "exit" ) == 0 ) return 0;
		if( ( childpid = fork() ) == 0 ){ // child function
			printf("command: %s\n",command[0]);
			printf("arg1: %s\n", command[1]); 
			if( execvp( command[0], command ) < 0 ){
				fprintf(stderr, "Exec of %s failed\n", command[0]);
				exit(1);
			}
		}else if( childpid > 0){		// parent function
			waitpid(childpid, &child_ret_val , 0);
			if((WEXITSTATUS(child_ret_val))){
				fprintf( stderr, "Error Code: %d\n", WEXITSTATUS(child_ret_val) );
			}

		}else{
			// if childpid == -1
			fprintf(stderr, "\nError in fork\n" );
			return 1;
		}
	}

}

void prompt(char *com[MAX_COMMAND][MAX_COMMAND]){
	printf( "?: " );
	int com_len = 0;
	int com_arr = 0;
	char c[MAX_COMMAND];
	char cc;
	int flag = 1;
	while( ( cc = getchar() ) != '\n' ){
		printf("char: %c\n", cc);
		c[com_len] = (char)cc;
		
		if(c[com_len] == ' '){
			flag = 1;
			c[com_len] = '\0';
			*com[com_arr]= c;
			com_arr = com_arr+1;
			com_len = 0;
			printf("part: %s\n", c);
		}else{
			com_len++;
		}
	}
	c[com_len] = '\0';
	printf("part: %s\n", c);
	*com[com_arr] = c;
	printf("part: %s\n", *com[com_arr]);
	//com[com_arr][com_len] = (char) NULL;
}
