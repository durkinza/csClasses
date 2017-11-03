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
int openFile(char * fn, int &k, int &v, int &p);
int getData(char * fn, int pfd, char * key, int q, int k, int v, int p);
void getHeaders(int pfd, int &k, int &v, int &p);
int setData(char * fn, int pfd, char * key, int noOver, int q, int k, int v, int p);
unsigned int hash(const char *str);
int d = 0; // debug option

int main(int argc, char*  argv[]){
	int c;
	char * file = NULL;
	int pfd;
	char * key = NULL;
	int action = 0;
	int show_max = 0;
	int noOverwrite = 0;
	int quiet=0;
	int k = 32;
	int p = 331;
	int v = 64;
	char help[] = "Usage: key {-s | -g} file key\n       key -[hnkvp] file\n";
	opterr = 0; // should stop getopt from giving it's own errors
	while ((c = getopt (argc, argv, "sgndhoqk:v:p:")) != -1){
		switch (c){
			case 'd':
				// set debugging on
				d = 1;
				break;
			case 'g':
				// for getting value from file
				action = 1; // set action to getting data
				break;
			case 'h':
				// send help
				printf("%s",help);
				exit(0);
				break;
			case 'k':
				// for setting key value size on new files
				if((int)*optarg > 57 || (int)*optarg < 48){
					printf("ERROR(key): number on -k option: %s is not legal.\n", optarg);
					if(d==1)printf("DEBUG(key): -k option: %d\n", k);
					exit(1);
				}
				k=atoi(optarg);
				break;
			case 'n':
				// for returning header values from file
				show_max = 1; // set action to returning headers
				break;
			case 'o':
				// Set to throw an error when over writing a previous record 
				noOverwrite = 1; // set action to returning headers
				break;
			case 'p':
				// for setting number of rows in a new file
				if((int)*optarg > 57 || (int)*optarg < 48){
					printf("ERROR(key): number on -p option: %s is not legal.\n", optarg);
					if(d==1)printf("DEBUG(key): -p option: %d\n", p);
					exit(1);
				}
				p=atoi(optarg);
				break;
			case 'q':
				// for silencing misuse messages
				quiet = 1;
				break;
			case 's':
				// for saving value to file
				action = 2; // set action to saving data
				break;
			case 'v':
				// for setting value size on new files
				if((int)*optarg > 57 || (int)*optarg < 48){
					printf("ERROR(key): number on -v option: %s is not legal.\n", optarg);
					if(d==1)printf("DEBUG(key): -v option: %d\n", v);
					exit(1);
				}
				v=atoi(optarg);
				break;
			default:
				printf("ERROR(key): option -%c is not an option\n", optopt);
				printf("%s",help);
				exit(1);
		}
	}
	int index = optind;
	if(index < argc){ // get file name from argument list
		file = argv[index];
		if(d==1)printf("DEBUG(key): opening %s for reading\n", file);
		//pfd = openFile(file, k, v, p);
		index++;
	}
	if(index < argc){  // get key from argument list
		key = argv[index];
		index++;
	}
	
	pfd = openFile(file, k, v, p);
	
	switch(action){
		case 1:			// action to just get data
			if(key==NULL){
				printf("ERROR(key): No key was specified for key file '%s'\n", file);
				exit(1);
			}
			getData(file, pfd, key, quiet, k, v, p);
			break;
		case 2:			// action to set data
			if(key==NULL){
				printf("ERROR(key): No key was specified for key file '%s'\n", file);
				exit(1);
			}
			setData(file, pfd, key, noOverwrite, quiet, k, v, p);
			break;
	}
	if(show_max){printf("%d %d %d\n", k, v, p);}
	close(pfd);
	return 0;
}

int makeFile(char * fn, int k, int v, int p){
	int pfd;
	if ((pfd = open(fn, O_CREAT|O_RDWR, S_IRUSR | S_IWUSR)) == -1){
    	printf("ERROR(key): Cannot create file '%s'\n", fn);
		exit(0);
	}else{
		char * mgbits;
		mgbits = (char*) malloc(4);
		if (mgbits==NULL) exit(1);
		mgbits[0]='K';
		mgbits[1]='E';
		mgbits[2]='Y';
		mgbits[3]='Z';
		// write the magic bits to the file
		write(pfd,  mgbits, sizeof(4)); //write(pfd, "KEYZ", sizeof(4));
		free(mgbits);
		
		int bits = write(pfd, &k, sizeof(int)); 
		write(pfd, &v, sizeof(int));
		write(pfd, &p, sizeof(int));
		if (bits == -1){
			printf("ERROR(key): Cannot create file '%s'\n", fn);
			if(d==1){ // debug
				printf("DEBUG(key): errno: %d\n",errno);
				printf("DEBUG(key): strerror: %s\n", strerror(errno));
			}
			exit(0);
		}
		 
		return pfd;
	}
}

int openFile(char * fn, int &k, int &v, int &p){
	int pfd;
	if ((pfd = open(fn, O_RDWR)) == -1){
		if((pfd = makeFile(fn, k, v, p)) == -1){
	    	printf("ERROR(key): Cannot create file '%s'\n", fn); 
			exit(1);
		}
		return pfd;
	}else{
		// verify that the file is correct before continuing
		char * key= (char *)malloc(5);
		read(pfd, key, 4);
		if(d==1)printf("DEBUG(key): magic bits are: %s\n", key); // debug
		if(std::strcmp(key, "KEYZ") ==0){
			getHeaders(pfd, k, v, p);
			return pfd;	
		}
		// if the magic bits are wrong, say so
		printf("ERROR(key): File '%s' is not of type key file.\n", fn);
		exit(1);
	}
}
void getHeaders(int pfd, int &k, int &v, int &p){
	read(pfd, &k, sizeof(int));
	read(pfd, &v, sizeof(int));
	read(pfd, &p, sizeof(int));
}

int getData(char * fn, int pfd, char * key, int q, int k, int v, int p){
	int h = hash(key);
	int rowSize = k+v+(2*sizeof(int));
	int position = ((h%p)*rowSize); 
	position = position < 0? -position: position;
	position += (4+(3*sizeof(int))); // find how many bits must be moved to get to the specified row, including header data
	lseek(pfd, position, SEEK_SET);
	int keyS;
	char * keyV = (char *) malloc(v);
	int rowsUsed=0;
	if((pread(pfd, &keyS, sizeof(int), position) < 0 ) || keyS == 0 ){
		if(q!=1)printf("ERROR(key): Trying to get value from key file '%s' for nonexistent record for key '%s'\n", fn, key);
		exit(1);
	}
	while( ( pread(pfd, &keyS, sizeof(int), position+(rowsUsed*rowSize)) 
		     && keyS != 0 
		   ) &&( 
				pread(pfd, keyV, keyS, position+(rowsUsed*rowSize)+(sizeof(int)*2)) &&
				(strcmp(keyV, key) !=0)
		   )
		)
	{
		rowsUsed++;
		if(d==1){
			printf("DEBUG(key): keyS: %d\n", keyS);
			printf("DEBUG(key): keyV: %s\n", keyV);
			printf("DEBUG(key): row used, moveing to row: %d\n", rowsUsed);
		}
	}
	lseek(pfd, position+(rowsUsed*rowSize), SEEK_SET);
	int keySize;
	int valueSize;
	read(pfd, &keySize, sizeof(int));
	read(pfd, &valueSize, sizeof(int));
	char * value = (char *)malloc(keySize);
	char * keyFound = (char *)malloc(valueSize);
	read(pfd, keyFound, keySize);
	read(pfd, value, valueSize);
	if(d == 1){
		printf("DEBUG(key): hash: %d\n", h);
		printf("DEBUG(key): position: %d\n",position);
		printf("DEBUG(key): keySize: %d\n", keySize);
		printf("DEBUG(key): keyS: %d\n", keyS);
		printf("DEBUG(key): valueSize: %d\n", valueSize);
		printf("DEBUG(key): keyFound: %s\n", keyFound);
	}
	printf("%s\n", value);
	free(value);
	free(keyFound);
	return 0;
}

int setData(char * fn, int pfd, char * key, int noOver, int q, int k, int v, int p){
	int h = hash(key);
	int rowSize = k+v+(2*sizeof(int));
	int position = ((h%p)*rowSize);
	position = position < 0? -position: position;
	position += (4+(3*sizeof(int))); // find how many bits must be moved to get to the specified row, including header data
	if(d == 1){ // debug
		printf("DEBUG(key): hash: %d\n", h);
		printf("DEBUG(key): position: %d\n", position);
	}
	lseek(pfd, position, SEEK_SET);
	int keySize = strlen(key); // using strlen because key is a pointer, so sizeof is always 8
	int keyS;
	char * keyV = (char *) malloc(v);
	int rowsUsed=0;
	while( ( (pread(pfd, &keyS, sizeof(int), position+(rowsUsed*rowSize)) != -1)
		     && keyS != 0 
		   ) &&( 
				pread(pfd, keyV, keyS, position+(rowsUsed*rowSize)+(sizeof(int)*2)) &&
				(strcmp(keyV, key) !=0)
		   )
		)
	{
		rowsUsed++;
		if(d==1)printf("DEBUG(key): row used, moveing to row: %d\n", rowsUsed);
	}
	if(noOver!=0  && (strcmp(keyV, key) == 0)){
		if(q!=1)printf("ERROR(key): Attempt to overwrite record for key '%s' when in no-overwrite mode\n", key);
		exit(1);
	}
	lseek(pfd, (rowsUsed*rowSize), SEEK_CUR);
	char value[v];
	char buf[1];
	int i =0;
	while( i<v && read(0, buf, sizeof(buf))>0) {// read from stdin character by character
		if(d==1)printf("b: %c\n", buf[0]);
		value[i] = buf[0];
		i++;	
	}
	value[i-1] = '\0';
	int valueSize = strlen(value); // using strlen because sizeof will return size of array and not size of string
	if(d==1){
		printf("DEBUG(key): key: %s\n", key);
		printf("DEBUG(key): keySize: %d\n", keySize);
		printf("DEBUG(key): value: %s\n", value);
		printf("DEBUG(key): valueSize: %d\n", valueSize);
	}
	write(pfd, &keySize, sizeof(int));	
	write(pfd, &valueSize, sizeof(int));
	write(pfd, key, keySize); 
	write(pfd, value, valueSize);

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
