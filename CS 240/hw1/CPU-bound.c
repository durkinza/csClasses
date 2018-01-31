#include <time.h>

int main(){
	int x;
	clock_t init, final;

	init=time(NULL);	
	final = init + ( 5 * 60 );
	while(time(NULL) < final ){
		x = x+1;
	}
}
