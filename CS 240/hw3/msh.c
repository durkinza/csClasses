/* My Shell
 * Author: Zane Durkin <durk7832@vandals.uidaho.edu>
 * CS 240
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_COMMAND 255
#define READ_END 0
#define WRITE_END 1

int crnt_history = 0;
char *path, *user;

void getCommand(FILE *);								// for reading commands from a file
void readrc();											// for reading mshrc file
void execute(char * command[], int in, int out, int err);							// for executing a command

typedef struct history {
    char value[MAX_COMMAND][MAX_COMMAND];
	int count;
	struct history * next;
} history;

typedef struct alias {
	char name[MAX_COMMAND];
	char value[MAX_COMMAND];
	struct alias * next;
} alias;

history * hist_tbl = NULL;								// create linked list for history
alias * alias_tbl = NULL;								// create linked list for aliases

history * history_lookup(int i);
history * history_add(char ** s);
void printHistory(int);
void printHistTbl();

char * alias_lookup(char * name);
void alias_add(char * name , char * value);
void alias_remove(char * name);
void printAliasTbl();


int main(){											
	path = getenv("PATH");
	user = getenv("USER");
	readrc();											// read in mshrc file
	char ** command;									// get ready to read commands
	while( 1 ){											// always read	
        printf( "?: ");									// print terminal pre-fix
		getCommand(stdin);								// read command
	} // end while(1)
} // end main

void getCommand(FILE *file){
	int pipefd[2];
	pipe(pipefd);
	int word_len = 0;									// holds word length
	int arr_len = 0;									// holds number of words
	int in_double_quotes = 0;							// if in double quotes
	int in_single_quotes = 0;							// if in single quotes
	int in_var = 0;										// if building variable starting with '$'
	char c[MAX_COMMAND];								// buffer for current word
	int another = 1;									// if there is another command on same line (seperated by ';')
	int pcount=0;											// how many commands are being piped
	int in = fileno(stdin);
	int out = fileno(stdout);
	int err = fileno(stderr);			// files for input, output, error
	char * command[MAX_COMMAND];
	while(another){
		word_len = 0;
		arr_len = 0;
		another = 0;
		while( ( c[word_len] = getc(file) ) != EOF )	{	// read until end of line
			if( ( c[word_len] == '\n' || (int)c[word_len] == 0x0a ) && !(in_single_quotes || in_double_quotes) ){
				break;
			}
			if ( !in_single_quotes && c[word_len]=='$'){
				//printf("$\n");
				in_var = 1;
				int eof =0;	
				while( (c[word_len] = getc(file)) != EOF){
					if( !( c[word_len] >=65 && c[word_len] <= 90) && !( c[word_len] >= 97 && c[word_len] <= 122) && c[word_len] != '_'){
						//printf("word: %s\n", c[word_len]);
						break;
					}
					word_len++;
				}
				in_var = 0;
				c[word_len] = '\0';
				//printf("word: %s word_len: %d\n", c, word_len);
				strcpy(c, getenv(c));
				int len = strlen(c);
				c[len+1] = '\0';
				//printf("word: %s len: %d\n", c, len);
				//printf("lastchar: |%c| %d\n", c[len], len);
				
				command[arr_len] = malloc(strlen(c)+1);		// create space for word in memory
				strcpy(command[arr_len], c);				// copy string into array
				command[arr_len][len] = '\0';
				arr_len++;									// increment words in array
				word_len = 0;								// reset character counter
				continue;
			}
			if ( c[word_len] == '\033'){
				getc(file); // skip the [
				switch(getc(file)) { // the real value
			        case 'A':
						// code for arrow up
						printHistory(++crnt_history);
						break;
					case 'B':
		        	    // code for arrow down
						printHistory(--crnt_history<0?0:crnt_history);
		        	    break;
					default:
						break;
				}
			}
			if ( c[word_len] == '"'){
				in_double_quotes = !in_double_quotes;
				continue;
			}
			if (c[word_len] == '\''){
				in_single_quotes = !in_single_quotes;
				continue;
			}
			if(c[word_len] == ';' && !(in_single_quotes || in_double_quotes) ){
				another = 1;
				break;
			}
			if(c[word_len] == '|' && !(in_single_quotes || in_double_quotes) ){
				//printf("pipe\n");
				if(pcount > 1)
					in = pipefd[READ_END];
				out = pipefd[WRITE_END];
				another = 1;
				pcount++;
				c[word_len] = '\0';
				command[arr_len] = malloc(word_len+1);
				strcpy(command[arr_len], c);
				break;	
			}
			if( c[word_len] == ' ' && !(in_single_quotes || in_double_quotes) ){
				//printf("space %d word: %d\n", c[word_len], word_len);
				if(word_len == 0){							// check that the word is not just a space
					continue;
				}
				c[word_len] = '\0';							// close each word
				command[arr_len] = malloc(word_len+1);		// create space for word in memory
				strcpy(command[arr_len], c);				// copy string into array
				arr_len++;									// increment words in array
				word_len = 0;								// reset character counter
			}else{
				//printf("letter %c %d\n", c[word_len], c[word_len]);
				word_len++;									// increment character counter
			}
		}
		if(in_single_quotes || in_double_quotes){
			printf("[ERROR] unfinished string \n");
			/*if(in_single_quotes)
				printf("in single\n");
			if(in_double_quotes)
				printf("in double\n");*/
			break;
		}	
		if( c[word_len] == ' ' || c[word_len] == '\n' || c[word_len] == ';' || c[word_len] == EOF){
			c[word_len] = '\0';								// be sure last argument ends with terminator
		}
		command[arr_len] = malloc(word_len+1);
		strcpy(command[arr_len], c);
		if (strlen(command[arr_len]) == 0)
			command[arr_len] = NULL;							// if last command is empty, just set it to null
		else
			command[arr_len+1]= NULL;							// be sure to set last array element to null
		if(!another)
			out = fileno(stdout);
		if(command[0] != NULL && command[0] != (char *)EOF)
		execute(command, in, out, err);
	}
}

void readrc(){
	FILE * rc;												// file pointer for mshrc file
    char letter;											// letter lookahead
	char path [255];
	strcpy(path, getenv("HOME"));
	strcat(path, "/.mshrc");								// set path to mshrc
    rc = fopen(path, "r");									// try to open the mshrc file
    if (rc == NULL)											// if the file cannot be opened
		printf("Could not load %s file\n", path);			// say file can't be opened
	else{													
		while( (letter = getc(rc) )){						// look a letter ahead
			if(letter == EOF || letter == '\0' || letter == -1){
				break;										// if at end of file, exit
			}else{
				printf(""); // needed for some reason, not sure why
			}
			ungetc(letter, rc);
			getCommand(rc);
		}
		fclose(rc);
	}
}

void execute(char * command[], int in, int out, int err){	
	pid_t childpid;
	int child_ret_val;
	if ( strlen(command[0]) < 1){
		return;
	}

	if( strcmp(command[0], "exit") == 0 || strcmp(command[0], "quit") == 0)// exit if requested 
		exit(0);

	
	if( strcmp(command[0], "history") == 0){			// show command history
		if(command[1] !=  NULL){
			printHistory(atoi(command[1]));
		}else{
			printHistTbl();
		}
		return;
	}

	
	if( command[0][0] == '!'){							// runn previous command, !! or {!1, ..., !N}
		char com [MAX_COMMAND][MAX_COMMAND];
		if(command[0][1] == '!'){						// if is !!, then run last command
			if(history_lookup(0) == NULL)
				return;
			memcpy(com,history_lookup(0)->value, 255*255);	
		}else{											// if is not !, then get number
			memmove(command[0], command[0]+1, strlen(command[0]));// move [!123] -> [123]
			int c = atoi(command[0]);					// convert to number
			if(history_lookup(c-1) == NULL)				// check that the history exists
				return;
			memcpy(com,history_lookup(c-1)->value, 255*255);// get item in history location
		}

		char * b[MAX_COMMAND];							// convert char[][] to char **
		for (int i =0; i < MAX_COMMAND; i++){			
			if(strcmp(com[i], "") != 0){
				if ((b[i] = malloc(sizeof(char) * sizeof(com[i]))) == NULL) {
					printf("unable to allocate memory \n");
				}else{
					strcpy(b[i], com[i]);
				}
			}else
				b[i] = NULL;	
		}
		char ** mother = b;								// now char [][] is char **
		printf("%s\n",mother[0]);				// say command that is running
		// normally would just print command to terminal
		// But not sure how to pre-fill terminal while restricted to getchar function
		execute(mother, in, out, err);								// execute command
		return;
	}
	history_add(command);								// add command to history

	if( strcmp(command[0], "cd") == 0){					// change directories
		if(command[1] !=  NULL){
			chdir(command[1]);							// go to given directory
		}else{
			chdir(getenv("HOME"));						// if no directory is give, go home
		}
		
		return;
	}
	if( strcmp(command[0], "export") == 0 ){
		if(command[1] != NULL){
			putenv(command[1]);
		}
	
	}
	if( strcmp(command[0], "alias") == 0){				// create alias
		if(command[1] != NULL){
			char * name =strtok(command[1], "=");
			char * value =strtok(NULL, "=");
			//printf("name: %s value: %s\n", name, value);
			alias_add(name, value);
			//alias_add(strtok(command[1], "="), strtok(NULL, "="));
		}else
			printAliasTbl();							// if no alias given, just print aliases
		return;
	}
	if( strcmp(command[0], "unalias") == 0){			// remove aliases
		if(command[1] != NULL){
			alias_remove(command[1]);
		}else
			printAliasTbl();							// if no alias given, just print aliases
		return;
	}
	
	command[0] = alias_lookup(command[0]);				// replace with alias, if it exists
	if( ( childpid = fork() ) == 0 ){// child function
		dup2(in, STDIN_FILENO);			// switch out stdin
		dup2(out, STDOUT_FILENO);		// switch out stdout
		dup2(err, STDERR_FILENO);		// switch out stderr
		if( execvp( command[0], command ) < 0 ){		// if exec failed, say why
			fprintf(stderr, "Exec of %s failed\n", command[0]);
			exit(1);									// exit with error
		}
		
		dup2(STDIN_FILENO, in);			// switch out stdin
		dup2(STDOUT_FILENO, out);		// switch out stdout
		dup2(STDERR_FILENO, err);		// switch out stderr

	}else if( childpid > 0){// parent function

		waitpid(childpid, &child_ret_val , 0);			// wait for child to return
		if( ( WEXITSTATUS(child_ret_val) ) ){			// get return value of child
			fprintf( stderr, "Error Code: %d\n", WEXITSTATUS(child_ret_val) );
		}

	}else{// error handler								// if fork failed
		fprintf(stderr, "\nError in fork\n" );			// say fork failed
		exit(1);

	}// end fork
}

/*
 * Alias Managment
 */

char * alias_lookup(char * s){							// return alias or given value
	alias * a = alias_tbl;
	while (a != NULL){									// move down list until end is reached
		if(strcmp(a->name, s) ==0)		
			return a->value;							// return alias, if it found
		a = a->next;
	}
	return s;											// default to the given name
}

void alias_add(char * name, char * value){				// add alias to table
	alias * tmp = malloc(sizeof(alias));				// make new alias
	strcpy(tmp->name, name);							// fill new alias struct	
	strcpy(tmp->value, value);
	tmp->next = alias_tbl;										// add alias to head
	alias_tbl = tmp;	
}

void alias_remove(char * name){							// remove alias from table
	alias * tmp = alias_tbl;
	alias * prev = alias_tbl;
	while(tmp != NULL){
		if(strcmp(tmp->name, name) == 0){
			if(tmp == alias_tbl){			// if it is the head
				alias_tbl = tmp->next;		// move head down
				tmp = alias_tbl;
				prev = alias_tbl;
			}else{
				prev->next = tmp->next;		// skip tmp
			}
		}else{
			prev= tmp;					// move down a location
			tmp = tmp->next;			// move down a location
		}
	}
}

void printAliasTbl(){									// print entire alias table
	alias * tmp = alias_tbl;
	while(tmp != NULL){
		printf("alias %s='%s'\n", tmp->name, tmp->value);
		tmp = tmp->next;	
	}
}

/*
 * History Managment
 */

history * history_add(char ** s){
	history * tmp= malloc(sizeof(history));				// clear out an empty symbol
	history * hi = hist_tbl;							// pointer transverse with
	int count = 0;
	while(s[count] != NULL)
		count++;
	tmp->count = count;
	for(int i = 0; i < count; i++) {
	    strcpy(tmp->value[i], s[i]); // allocates the second dimension
	}
	tmp->next = hi;										// set new->next to the head
	hist_tbl = tmp;
	return tmp;
}

history * history_lookup(int i){						// return history item, or null
	history * h = hist_tbl;
	for(int j = 0; j < i; j++){
		if(h == NULL)
			return NULL;
		h = h->next;
	}
	return h;
}

void printHistory(int i){								// print given history item
	history * tmp = history_lookup(i);
	if(tmp != NULL)
	for(int i=0; i < tmp->count; i++){
		printf("%s", tmp->value[i]);
	}
	printf("\n");
}

void printHistTbl(){									// print last 20 history items
	history * tmp = hist_tbl;
	for(int i=19; i>=0; i--){
		printHistory(i);
	} 
}

