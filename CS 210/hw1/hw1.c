/* Zane C. Durkin
 * CS 210 
 * Hw1
 */

//print_until(char *st[], char *line[]);
//print_while(char *st[], char *line[]);

int main(int argc, char *argv[]){
	if(argc < 2){
		printf("missing input file");
		return 0;
	}
	printf("Hello, world\n");
	for(int i=argc-1; i>0; i--){
		printf("arg[%d]: %s\n", i, argv[i]);
		
	}
}
