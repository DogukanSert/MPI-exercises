#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>


int main(int argc, char * argv[]){
if ( argc != 2){
	printf( "Incorrect usage");
}
else{
	clock_t t;
	t = clock();
	int n = atoi(argv[1]);
	double sum = 0.0;
	double dx = 1.0/n;
	double x,mid;
	int i;
	for(i = 0; i < n; i++){
		mid = dx * (i + 0.5);
		x = 1.0 / (1.0 + mid*mid);
		sum += x;
	}
	sum = 4 * sum * dx;
	printf("pi app = %.15f\n", sum);
	t = clock() - t;
	printf ("Program took %f seconds\n",((float)t)/CLOCKS_PER_SEC);
	return 0;
}
}
