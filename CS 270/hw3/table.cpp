#include<stdio.h>
#include"funcs.h"
#include"table.h"
int table(double start, double end, double step){
	for(double x=start; x<=end; x+=step){
		printf("f(%f): %f\n",x,f(x));
		printf("g(%f):%f\n",x,g(x));
	}
	return 0;
}
