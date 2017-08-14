#include <mpi.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>

int main (int argc, char* argv[]){
if ( argc != 2){
	printf( "Incorrect usage");
}
else{
	MPI_Status s;
	int rank, size;
	MPI_Init (&argc, &argv);      /* starts MPI */
 	MPI_Comm_rank (MPI_COMM_WORLD, &rank);        /* get current process id */
  	MPI_Comm_size (MPI_COMM_WORLD, &size);        /* get number of processes */
	int n = atoi(argv[1]);
	double start;
	if(rank == 0){
		start = MPI_Wtime();
	}

	int low = (rank*n)/size;
	int high =  floor(((rank+1)*n)/size);
	int *primesLocal = (int*) malloc(n * sizeof(int));
	int i;
	for(i = 0; i<n; i++){primesLocal[i] = 0;}
	
	int prime = 2 ;

	while( prime *prime <= n){
		for(i = low; i<high;i++){
			if(i % prime == 0 && i != prime ){primesLocal[i] = 1;}
		}
		if(rank == 0){
			while(primesLocal[++prime]);
		}
		MPI_Bcast(&prime,1,MPI_INT,0,MPI_COMM_WORLD);
	}

	int * primes = (int*) malloc(n * sizeof(int));
	for(i = 0; i<n; i++){primes[i] = 0;}
	MPI_Allreduce(primesLocal,primes,n,MPI_INT,MPI_SUM,MPI_COMM_WORLD);
	if(rank == 0){
		for(i = 0; i < n;i++){
			if(primes[i] == 0){
				printf("%d \n", i);
			}
		}
		double end =MPI_Wtime();
		printf("Program took %.5f seconds\n",end-start);
	}
	MPI_Finalize();
	return 0;
}
}
