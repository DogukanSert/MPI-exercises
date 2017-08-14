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
	int primes[n];
	int i,j;
	for(i = 0; i < n;i++){
		primes[i] = 1;
	}

	primes[0] = 0;

	for(i = 2; i < sqrt(n); i++){
		for(j=i*i; j < n; j+=i){
			primes[j] = 0;
			
		}
	}
	t = clock() - t;

	for(i = 0; i < n;i++){
		if(primes[i] == 1){
			printf("%d ", i);
		}
	}
	printf("\n");
	printf ("Program took %f seconds\n",((float)t)/CLOCKS_PER_SEC);	
	return 0;
}
}
