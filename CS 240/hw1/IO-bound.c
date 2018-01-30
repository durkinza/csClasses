#include <time.h>
#include <stdio.h>
#include <stdlib.h>

int main(){

	int x;
	clock_t init, final;
	FILE *fp;
	fp=fopen("IO-output.txt", "w");
	init=time(NULL);	
	final = init + (10);
	
	while(time(NULL) < final ){
		fseek(fp, 0, SEEK_SET);
		fprintf(fp, "A");
	}
	fclose(fp);
}
