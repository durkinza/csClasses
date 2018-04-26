/* 
 * Author: Zane Durkin <durk7832@vandals.uidaho.edu>
 * CS 240
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <time.h>
#include <signal.h>

#include <sys/shm.h>

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
	// read in command line arguments
	int time = atoi(argv[1]);
	int max_p = atoi(argv[2]);
	int max_c = atoi(argv[3]);
	
	// track process ids
	int processes[NUM_PROD];
	int consumers[NUM_CON];
	int buff_pid;

	// create array of pipes
	int p_pipe[NUM_PROD + NUM_CON][2];
	
	printf("time = %d, max_p = %d, max_c = %d\n", time, max_p, max_c);	
	// create pipes
	for(int i = 0; i < NUM_PROD + NUM_CON; i++){
		pipe(p_pipe[i]);
	}	

	// start buffer process
	buff_pid = start_buffer(p_pipe, time);
	printf("created buffer: %d\n", buff_pid);


	// start consumers
	for(int i = 0; i < NUM_CON; i++){
		consumers[i] = start_con(p_pipe[i], i, time, max_c);
		printf("created consumer: %d - pid: %d\n", i+1, consumers[i]);

	}

	// start producers
	for(int i = 0; i < NUM_PROD; i++){
		processes[i] = start_prod(p_pipe[i+NUM_CON], i, time, max_p);
		printf("created producer: %d - pid: %d\n", i+1, processes[i]);
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
        // code that runs in the child process
        // get a new random seed
		srand(time(NULL) ^ (getpid()<<16));
		// find total number of processes
		int total_p = NUM_PROD + NUM_CON;
		// note start time 
		time_t start = time(0);
		
		int endCounter = 0;
		int startCounter = 0;
		char buffer[BUFF_SIZE];
		double average;
		int wait = 20;

		// shared memory for producer flags
		int * p_flags[NUM_PROD];
		int p_flags_ids[NUM_PROD];
		
		// shared memory for consumer flags
		int * flags[NUM_CON];
		int flags_ids[NUM_CON];
		int shmid;
	
		// setup shared memory for consumers
		for ( int i =0; i< NUM_CON; i++){
			int * shm;
			flags_ids[i] = shmget(2000 + i, sizeof(int), 0666 | IPC_CREAT);
			shm = shmat(flags_ids[i], 0, 0);
			flags[i] = (int *)shm;
		}

		// setup shared memory for producers
		for ( int i =0; i< NUM_PROD; i++){
			int * shm;
			p_flags_ids[i] = shmget(3000 + i, sizeof(int), 0666 | IPC_CREAT);
			shm = shmat(p_flags_ids[i], 0, 0);
			p_flags[i] = (int *)shm;
		}

		// do the buffer stuff
        while( difftime( time(0), start) < (double)max_time){
			// check the producers to add to the buffer
			for(int i = 0; i< NUM_PROD; i++){
				// producers will set flag when they have written something
				if(*p_flags[i] == 1){
					// set flag to 0
					*p_flags[i] = 0;
					// increment end of buffer pointer, being sure to go back to start when needed
					endCounter = (endCounter + 1) % BUFF_SIZE;
					// read in value from producer
					read(fd[i+NUM_CON][READ], &buffer[endCounter], sizeof(char));
				}
			}
			// check consumers to read from the buffer
			for(int i = 0; i < NUM_CON; i++){
				// flag will be 1 when consumer is ready to read ( a non-blocking approach)
				if(*flags[i] == 1){
					// set flag back to 0
					*flags[i] = 0;
					// move start of buffer pointer to next position, going back to start when needed
					startCounter = (startCounter + 1) % BUFF_SIZE;
					// write char from buffer into consumer
					write(fd[i][WRITE],&buffer[startCounter], sizeof(char));
				}
			}
			
			// calculater average buffer size
			if(endCounter < startCounter){
				// if the end pointer has gone back to the start of the array
				// this will take value from start pointer to end of array and then add the value of end pointer
				average = ( (BUFF_SIZE - startCounter + endCounter) + average )/2;
			}else{
				// this will take the average normally
				average = ( (endCounter - startCounter) + average )/2;
			}
				
        }
		//printf("buffer exited\n");
		// close pipes
		for(int i = 0; i < total_p; i++){
			close(fd[i][WRITE]);
			close(fd[i][READ]);
		}
		// close consumer flags
		for ( int i =0; i< NUM_CON; i++){
			shmdt(flags[i]);
			shmctl(flags_ids[i], IPC_RMID, NULL);
		}
		// close producer flags
		for ( int i =0; i< NUM_PROD; i++){
			shmdt(p_flags[i]);
			shmctl(p_flags_ids[i], IPC_RMID, NULL);
		}
		// print average size
		printf("Average Buffer Size: %f\n", average);
		exit(0);
	}
	return pid;
}

int start_prod(int fd[2], int id, int max_time, int max_p){
	// function for starting a producer
	pid_t pid;
	pid = fork();
	int * shm;
	int shmid;

    if (pid < 0) {
        printf("fork prod error %d\n", pid);
        return -1;
        }
    else if (pid == 0) {
		srand(time(NULL) ^ (getpid()<<16));
		// create a shared memory for flag
		// flag will tell buffer when producer has produced a char
		int * flg;	
		shmid = shmget(3000 + id, sizeof(int), 0);
		shm = shmat(shmid, 0, 0);
		flg = (int *)shm;
        // code that runs in the child process
		time_t start = time(0);
		clock_t elapse = clock();
		int wait =  rand()%(max_p + 1)+1; // don't allow a zero waiting time
		
		char letter;
        close(fd[READ]); // close reading side
		
		// do producer stuff
        while( difftime( time(0), start) < (double)max_time){
			// make a random char
			letter = 'A' + (rand() % 26);
			// show the console
			printf("Producer %d: value :%c, tp=%dms\n", id+1, letter, wait);
			// tell buffer that something will be written
			*flg = 1;
			// write the char
			write(fd[WRITE], &letter, sizeof(letter));
			// sleep for set amount of ms
			usleep(wait * 1000);
		}
		
		// close buffer when done
        close(fd[WRITE]);
		//printf("prod %d exited\n", id);
		exit(0);
	}
	return pid;
}

int start_con(int fd[2], int id, int max_time, int max_c){
	// function for starting a consumer
	pid_t pid;
	pid = fork();
	int * shm;
	int shmid;

    if (pid < 0) {
        printf("fork con error %d\n", pid);
        return -1;
        }
    else if (pid == 0) {
		// get a new random seed
		srand(time(NULL) ^ (getpid()<<16));
		// create a shared memory for flag
		// flag will tell buffer when consumer is ready to recieve a char
		int * flg;	
		shmid = shmget(2000 + id, sizeof(int), 0);
		shm = shmat(shmid, 0, 0);
		flg = (int *)shm;
        // code that runs in the child process
		time_t start = time(0);
		clock_t elapse = clock();
		int wait =  rand()%(max_c + 1)+1; // don't allow a zero waiting time
		
		char letter;
		close(fd[WRITE]); // close writing side.
		// do consumer stuff
        while( difftime( time(0), start) < (double)max_time){
			// tell buffer consumer is ready to read
			*flg = 1;
			// read from buffer
			read(fd[READ], &letter, sizeof(char));
			// tell console
			printf("Consumer %d: value :%c, tp=%dms\n", id+1, letter, wait);
			// wait for set amount of ms
			usleep(wait * 1000);
		}
		// close pipe
		close(fd[READ]);	
		//printf("con %d exited\n", id);
		exit(0);
	}
	return pid;
}
