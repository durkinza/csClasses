#include <math.h>
#include "table.h"
double f(double x){
	return sqrt(x);
}
double g(double x){
	double t=0.00;
	for(double i=0.00; i<=x; i++){
		t += pow(x,i);
	}
	return t;
}
