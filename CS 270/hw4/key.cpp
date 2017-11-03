/* Author: Zane Durkin <durk7832@vandals.uidaho.edu>
 * File: key.cpp
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <cstring>// used for strcmp
#include <cerrno>
#include <fcntl.h>


// define functions
unsigned int hash(const char *str);
int makeFile(char * fn, int k, int v, int p);
int openFile(char * fn, int &k, int &v, int &p);
int getData(char * fn, int pfd, char * key, int q, int k, int v, int p);
void getHeaders(int pfd, int &k, int &v, int &p);
int setData(char * fn, int pfd, char * key, int noOver, int q, int k, int v, int p);
unsigned int hash(const char *str);
int d = 0; // global debug option

int main(int argc, char*  argv[]){
	int c;										// variable for holding argument
	char * file = NULL;							// Holds requested file name
	int pfd;									// Holds file pointer
	char * key = NULL;							// Holds given key to either search for or set
	int action = 0;								// action is set to get data(1) or to set data (2)
	int show_max = 0;							// set to show k, p, v values to false
	int noOverwrite = 0;						// set no Overwrite to false
	int quiet=0;								// set quite messages to false
	int k = 32;									// set default value for k
	int p = 331;								// set default value for p
	int v = 64;									// set default value for v
	char help[] = "Usage: key {-s | -g} file key\n       key -[hnkvp] file\n";
	opterr = 0;									// should stop getopt from giving it's own errors
	while ((c = getopt (argc, argv, "sgndhoqk:v:p:")) != -1){
		switch (c){
			case 'd':
				d = 1;							// set debugging on
				break;
			case 'g':							// for getting value from file
				action = 1;						// set action to getting data
				break;
			case 'h':							// if they use help option
				// send help
				printf("%s",help);				// print out help
				exit(0);						// leave program
				break;
			case 'k':							// for setting key value size on new files
				if( (int)*optarg > 57 || (int)*optarg < 48)
				{								// check that the k option given is a number
					printf("ERROR(key): number on -k option: %s is not legal.\n", optarg);
					if(d==1)printf("DEBUG(key): -k option: %d\n", k);
					exit(1);					// print an error if k is not a number, and leave program
				}
				k=atoi(optarg);
				break;
			case 'n':							// for returning header values from file
				show_max = 1;					// set show max values to true
				break;
			case 'o':							// Set to throw an error when over writing a previous record 
				noOverwrite = 1;				// set no Overwrite to true, so warnings will show if they try to overwrite an already set value
				break;
			case 'p':							// for setting number of rows in a new file
				if((int)*optarg > 57 || (int)*optarg < 48)
				{								// check that the p option is a number
					printf("ERROR(key): number on -p option: %s is not legal.\n", optarg);
					if(d==1)printf("DEBUG(key): -p option: %d\n", p);
					exit(1);					// print an error if p is not a number, and leave program
				}
				p=atoi(optarg);
				break;
			case 'q':							// for silencing misuse messages
				quiet = 1;						// set quiet to true
				break;
			case 's':							// for saving value to file
				action = 2;						// set action to saving data
				break;
			case 'v':							// for setting max value size on new file
				if((int)*optarg > 57 || (int)*optarg < 48)
				{								// check that the v option is a number
					printf("ERROR(key): number on -v option: %s is not legal.\n", optarg);
					if(d==1)printf("DEBUG(key): -v option: %d\n", v);
					exit(1);					// print an error if v is not a number, and leave program
				}
				v=atoi(optarg);
				break;
			default:							// if an unknown option was given
				printf("ERROR(key): option -%c is not an option\n", optopt);
				printf("%s",help);				// pring an error on the improper argument
				exit(1);						// leave program
		}
	}
	int index = optind;
	if(index < argc){							// get file name from argument list
		file = argv[index];
		if(d==1)printf("DEBUG(key): opening %s for reading\n", file);
		index++;								// increment index to show that the filename was retrieved
	}
	if(index < argc){							// get key from argument list
		key = argv[index];
		index++;								// increment index to show that key was retrieved
	}
	
	pfd = openFile(file, k, v, p);				// open the choosen file, or create it if it doens't exist
	
	if(show_max)printf("%d %d %d\n", k, v, p);	// print max values if that option was set
	if (action != 0){
		if(pfd == -1){
			printf("ERROR(key): Cannot create file '%s'\n", file);
			exit(1);
		}
		if(key==NULL){
			printf("ERROR(key): No key was specified for key file '%s'\n", file);
			exit(1);						// if the key is missing, throw an error and leave
		}
	}

	switch(action){
		case 1:									// action to just get data
			getData(file, pfd, key, quiet, k, v, p);
			break;
		case 2:									// action to set data
			setData(file, pfd, key, noOverwrite, quiet, k, v, p);
			break;
	}
	close(pfd);									// close the file 
	return 0;									// return 0, leave program
}

int makeFile(char * fn, int k, int v, int p){	// for making the key file if it doens't exist
	int pfd;
	if ((pfd = open(fn, O_CREAT|O_RDWR, S_IRUSR | S_IWUSR)) == -1){
		return pfd;								// if a file cannot be made, throw an error
	}else{										// if file is made and opened successfully
		int bits = write(pfd, "KEYZ", sizeof(4));// set magic bits of file
		write(pfd, &k, sizeof(int));			// write k value to file
		write(pfd, &v, sizeof(int));			// write v value to file
		write(pfd, &p, sizeof(int));			// write p value to file
		if (bits == -1){						// check that the first value could be written to
			printf("ERROR(key): Cannot create file '%s'\n", fn);
			if(d==1){ // debug
				printf("DEBUG(key): errno: %d\n",errno);
				printf("DEBUG(key): strerror: %s\n", strerror(errno));
			}
			exit(0);							// leave program is values cannot be set
		}
		return pfd;
	}
}

int openFile(char * fn, int &k, int &v, int &p){
	int pfd;
	if ((pfd = open(fn, O_RDWR)) == -1){		// try to open file normally
		pfd = makeFile(fn, k, v, p);			// try to make file if it doesn't open
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
		if(d==1){ // debug
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
	if(d == 1){ // debug
		printf("DEBUG(key): hash: %d\n", h);
		printf("DEBUG(key): position: %d\n",position);
		printf("DEBUG(key): keySize: %d\n", keySize);
		printf("DEBUG(key): keyS: %d\n", keyS);
		printf("DEBUG(key): valueSize: %d\n", valueSize);
		printf("DEBUG(key): keyFound: %s\n", keyFound);
	}
	printf("%s", value);
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
	char * value = (char *)malloc(v);
	char buf[1];
	int i =0;
	while( i<=v && read(0, buf, sizeof(buf))>0) {// read from stdin character by character
		if(d==1)printf("DEBUG(key) buf: %c\n", buf[0]);
		value[i] = buf[0];
		i++;	
	}
	int valueSize = strlen(value); // using strlen because sizeof will return size of array and not size of string
	if(d==1){ // debug
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
