#include <time.h>
#include <stdio.h>
#include <stdlib.h>

int main(){

	int x;
	clock_t init, final;
	FILE *fp;
	init=time(NULL);	
	final = init + (5 * 60);
	
	while(time(NULL) < final ){
		fp=fopen("IO-output.txt", "w");
		fseek(fp, 0, SEEK_SET);
		//fprintf(fp, "A");
		putc('a', fp);
		fclose(fp);
		fflush(fp);
	}
}
