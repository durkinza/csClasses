/* Author: Zane Durkin <durk7832@vandals.uidaho.edu>
 * File: key.cpp
 * 
 */
//#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <cstring>// used for strcmp
#include <cerrno>
#include <fcntl.h>


//#include <string.h>

unsigned int hash(const char *str);
int makeFile(char * fn, int k, int v, int p);
int openFile(char * fn, int k, int v, int p);
int getData(int pfd, char * key);
int getHeaders(int pfd);
int setData(int pfd, char * key, char * value);
unsigned int hash(const char *str);

int main(int argc, char*  argv[]){
	int c;
	char * file;
	int pfd;
	char * key;
	char * saveValue = NULL;
	int action = 0;
	int k = 32;
	int p = 331;
	int v = 64;
	int d = 0; // debug option
	char help[] = "Usage: key {-s | -g} file key\n       key -[hnkvp] file\n";
	opterr = 0; // should stop getopt from giving it's own errors
	while ((c = getopt (argc, argv, "sgndhk:v:p:")) != -1){
		switch (c){
			case 'g':
				// for getting value from file
				action = 1; // set action to getting data
				break;
			case 's':
				// for saving value to file
				action = 2; // set action to saving data
				break;
			case 'n':
				//for returning header values from file
				action = 3; // set action to returning headers
				break;
			case 'k':
				// for setting key value size on new files
				k=atoi(optarg);
				break;
			case 'v':
				// for setting value size on new files
				v=atoi(optarg);
				break;
			case 'p':
				// for setting number of rows in a new file
				p=atoi(optarg);
				break;
			case 'd':
				// set debugging on
				d = 1;
			case 'h':
				// send help
				printf("%s",help);
				exit(0);
			default:
				printf("ERROR(key): option -%c is not an option\n", optopt);
				printf("%s",help);
				exit(0);
		}
	}
	int index = optind;
	if(index < argc){
		file = argv[index];
		//printf("opening %s for reading\n", file);
		pfd = openFile(file, k, v, p);
		/*
		if(pfd == -1){
			printf("ERROR(key): Cannot create file '%s'\n", file);
			exit(1);
		}
		printf("opened file\n");*/
		index++;
	}
	if(index < argc){
		key = argv[index];
		index++;
	}
	

	switch(action){
		case 0:			// if nothing is specified, then try to make the file
			pfd = makeFile(file, k, v, p);
			break;
		case 1:			// action to just get data
			pfd = openFile(file, k, v, p);
			getData(pfd, key);
			break;
		case 2:			// action to set data
			pfd = openFile(file, k, v, p);
			setData(pfd, key, saveValue);
			break;
		case 3:			// action to get header data of file
			//printf("opening File\n");
			pfd = openFile(file, k, v, p);
			//printf("getHeaders\n");
			getHeaders(pfd);
			break;
	}

	close(pfd);
}

int makeFile(char * fn, int k, int v, int p){
	int pfd;
	if ((pfd = open(fn, O_CREAT|O_RDWR, S_IRUSR | S_IWUSR)) == -1){
    	printf("ERROR(key): Cannot create file '%s'\n", fn);
		exit(0);
	}else{
		char * mgbits;
		mgbits = (char*) malloc (4);
		if (mgbits==NULL) exit (1);
		mgbits[0]='K';
		mgbits[1]='E';
		mgbits[2]='Y';
		mgbits[3]='Z';
		// write the magic bits to the file
		write(pfd,  mgbits, sizeof(4));
		//write(pfd, "KEYZ", sizeof(4));
		free(mgbits);
		
		int bits = write(pfd, &k, sizeof(int)); 
		write(pfd, &v, sizeof(int));
		write(pfd, &p, sizeof(int));
		if (bits == -1){
			printf("ERROR(key): Cannot create file '%s'\n", fn);
			//printf("errno: %d\n",errno);
			//printf("%s\n", strerror(errno));
			exit(0);
		}
		 
		return pfd;
	}
}

int openFile(char * fn, int k, int v, int p){
	int pfd;
	if ((pfd = open(fn, O_RDWR)) == -1){
		if((pfd = makeFile(fn, k, v, p)) == -1){
	    	printf("ERROR(key): Cannot create file '%s'\n", fn); 
			exit(1);
		}
		return pfd;
	}else{
		// verify that the file is correct before continuing
		char key[5];
		read(pfd, key, 4);
		//printf("key is: %s\n", key);
		if(std::strcmp(key, "KEYZ") ==0){
			return pfd;	
		}
		// if the magic bits are wrong, say so
		printf("ERROR(key): File '%s' is not of type key file.\n", fn);
		exit(1);
	}
}

int getHeaders(int pfd){
	int k, v, p; 
	//printf("getting headers\n");
	read(pfd, &k, sizeof(int));
	read(pfd, &v, sizeof(int));
	read(pfd, &p, sizeof(int));
	printf("%d %d %d", k, v, p);
	return 0;
}

int getData(int pfd, char * key){
	return 0;
}

int setData(int pfd, char * key, char * setValue){
	int h = hash(key);
	printf("%d", h);
	
	return 0;
}

// Hash modified by Robert Heckendorn based on work 
// By Arash Partow (http://www.partow.net/programming/hashfunctions/)
//
// An algorithm produced by me Arash Partow. I took ideas from all of
// the above hash functions making a hybrid rotative and additive hash
// function algorithm based around four primes 3,5,7 and 11.  There isn't
// any real mathematical analysis explaining why one should use this
// hash function instead of the others described above other than the
// fact that I tired to resemble the design as close as possible to a
// simple LFBSR.  An empirical result which demonstrated the distributive
// abilities of the hash algorithm was obtained using a hash-table with
// 100003 buckets, hashing The Project Gutenberg Etext of Webster's
// Unabridged Dictionary, the longest encountered chain length was 7,
// the average chain length was 2, the number of empty buckets was 4579.
unsigned int hash(const char *str)
{
    unsigned int hash;
    char c;

    hash = 0;
    for (bool toggle = false; (c = *str++); toggle = !toggle) {
        if (toggle)
            hash ^= (hash << 7)^c^(hash >> 3);
        else
            hash ^= ~(hash << 11)^c^(hash >> 5);
    }

    return hash;
}
