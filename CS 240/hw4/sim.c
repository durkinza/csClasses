/* 
 * Author: Zane Durkin <durk7832@vandals.uidaho.edu>
 * CS 240
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

#define BUFF_SIZE 1000
#define NUM_PROD 4
#define NUM_CON 4
#define READ 0
#define WRITE 1

int start_buffer(int fd[NUM_PROD + NUM_CON][2], int max_time);
int start_prod(int fd[2], int id, int max_time, int max_p);
int start_con(int fd[2], int id, int max_time, int mac_c);


int main(int argc, char *argv[]){
	if( argc != 4 ) {
		printf("Incorrect number of arguments supplied.\n");
		exit(1);
	}
	int time = atoi(argv[1]);
	int max_p = atoi(argv[2]);
	int max_c = atoi(argv[3]);
	int processes[NUM_PROD];
	int consumers[NUM_CON];
	int buff_pid;
	int p_pipe[NUM_PROD + NUM_CON][2];
	
	printf("time = %d, max_p = %d, max_c = %d\n", time, max_p, max_c);	
	for(int i = 0; i < NUM_PROD + NUM_CON; i++){
		pipe(p_pipe[i]);
	}	
	//printf("created pipes\n");

	buff_pid = start_buffer(p_pipe, time);

	printf("created buffer: %d\n", buff_pid);
	
	for(int i = 0; i < NUM_CON; i++){
		// start consumers
		consumers[i] = start_con(p_pipe[i], i+1, time, max_c);
		printf("created consumer: %d\n", consumers[i]);

	}
	for(int i = 0; i < NUM_PROD; i++){
		// start producers
		processes[i] = start_prod(p_pipe[i+NUM_CON], i+1, time, max_p);
		printf("created producer: %d\n", processes[i]);
	}
    int status;
    if (waitpid(buff_pid, &status, 0) > 0)
    {
        if (WIFEXITED(status))
        {
        }   
        else if (WIFSIGNALED(status))
        {
        }
    }
}


int start_buffer(int fd[NUM_PROD + NUM_CON][2], int max_time){
	// function for starting the buffer
	pid_t pid;
	pid = fork();
	if (pid < 0) {
        printf("fork error %d\n", pid);
        return -1;
        }
    else if (pid == 0) {
		srand(time(NULL));
        // code that runs in the child process
		int total_p = NUM_PROD + NUM_CON;
		time_t start = time(0);
		
		int endCounter = 0;
		int startCounter = 0;
		char buffer[BUFF_SIZE];
		double average;
		int wait = 20;

        while( difftime( time(0), start) < (double)max_time){

			time_t elapse = time(0);// temp
			for(int i = 0; i< NUM_PROD; i++){
				read(fd[i+NUM_CON][READ], &buffer[endCounter++], sizeof(char));
			}
			printf("buffer read value:%c\n", buffer[endCounter-1]);
			for(int i = 0; i < NUM_CON; i++){
				write(fd[i][WRITE],&buffer[startCounter++], sizeof(char));
			}
			
			// reset counters if needed
			endCounter = endCounter % BUFF_SIZE;
			startCounter = startCounter % BUFF_SIZE;
			//printf("The value of startCounter is %d\n", startCounter);
			//printf("The value of endCounter is %d\n", endCounter);
			average = ( (endCounter - startCounter) + average )/2;
			printf("Average Buffer Size: %f\n", average);
				
        }
		for(int i = 0; i < total_p; i++){
			close(fd[i][WRITE]);
			close(fd[i][READ]);
		}
		printf("buffer exited\n");
		printf("Average Buffer Size: %f\n", average);
		exit(0);
	}
	return pid;
}

int start_prod(int fd[2], int id, int max_time, int max_p){
	// function for starting a producer
	pid_t pid;
	pid = fork();

    if (pid < 0) {
        printf("fork error %d\n", pid);
        return -1;
        }
    else if (pid == 0) {
		srand(time(NULL));
        // code that runs in the child process
		time_t start = time(0);
		clock_t elapse = clock();
		int wait =  rand()%(max_p + 1)+1; // don't allow a zero waiting time
		
		char letter;
        close(fd[READ]); // close reading side
		//printf("prod waiting: %lu\n", (unsigned long)wait);
		
        while( difftime( time(0), start) < (double)max_time){
			//clock_t wait = clock();
			letter = 'A' + (rand() % 26);
			printf("Producer %d: value :%c, tp=%dms\n", id, letter, wait);
			write(fd[WRITE], &letter, sizeof(letter));
			usleep(wait * 1000);
		}
		
        close(fd[WRITE]);
		printf("prod %d exited\n", id);
		exit(0);
	}
		return pid;
}

int start_con(int fd[2], int id, int max_time, int max_c){
	// function for starting a consumer
	pid_t pid;
	pid = fork();

    if (pid < 0) {
        printf("fork error %d\n", pid);
        return -1;
        }
    else if (pid == 0) {
		srand(time(NULL));
        // code that runs in the child process
		time_t start = time(0);
		clock_t elapse = clock();
		int wait =  rand()%(max_c + 1)+1; // don't allow a zero waiting time
		
		char letter;
		close(fd[WRITE]); // close writing side.
		
        while( difftime( time(0), start) < (double)max_time){
			read(fd[READ], &letter, sizeof(char));
			printf("Consumer %d: value :%c, tp=%dms\n", id, letter, wait);
			usleep(wait * 1000);
		}
		close(fd[READ]);	
		printf("con %d exited\n", id);
		exit(0);
	}
	return pid;
}
