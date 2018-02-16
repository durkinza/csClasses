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

void prompt(char *(*command)[MAX_COMMAND] );// output '?:' and wait for response
int main(){
	pid_t childpid;
	char* command[MAX_COMMAND];
	int com_len, child_ret_val;
	while( 1 ){
		//prompt(&command);
		//*
		printf( "?: " );
		int com_len = 0;
		int com_arr = 0;
		char c[MAX_COMMAND];
		while( ( c[com_len] = getchar() ) != '\n' )	{		// read until end of line
			if( c[com_len] == ' ' ){						// read until word ends

				c[com_len] = '\0';							// close each word
				command[com_arr] = malloc(com_len);			// create space for word in memory
				strcpy(command[com_arr], c);				// copy string into array
				com_arr = com_arr+1;						// increment words in array
				com_len = 0;								// reset character counter
				
			}else{
				com_len++;									// increment character counter
			}
		}
		if( c[com_len] == ' ' || c[com_len] == '\n' )
			c[com_len] = '\0';								// be sure last argument ends with terminator
		command[com_arr] = c;								// copy over last argument
		command[com_arr+1]= NULL;							// be sure to set last array element to null
//*/
		if( strcmp(command[0], "exit")==0 )					// exit if requested 
			return 0;

		if( ( childpid = fork() ) == 0 ){ // child function
			printf("after fork command: %s\n",command[0]);
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

void prompt(char *(*command)[MAX_COMMAND]){
	printf( "?: " );
	int com_len = 0;
	int com_arr = 0;
	char c[MAX_COMMAND];
	while( ( c[com_len] = getchar() ) != '\n' ){
		if( c[com_len] == ' ' ){
			c[com_len] = '\0';
			*command[com_arr] = malloc(com_len);
			strcpy(*command[com_arr], c);
			com_arr = com_arr+1;
			com_len = 0;		
		}else{
			com_len++;									//
		}
	}
	if( c[com_len] == ' ' || c[com_len] == '\n' )
		c[com_len] = '\0';								// be sure last argument ends with terminator
	*command[com_arr] = malloc(com_len);
	strcpy(*command[com_arr], c);						// copy over last argument
	*command[com_arr+1]= NULL;							// be sure to set last array element to null
	printf("%s\n", c);
	printf("%s\n", *command[com_arr]);
}
