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
	if(rank == 0){
		double start = MPI_Wtime();
		int n = atoi(argv[1]);
		int blockSize = n/size;
		int i;
		for(i = 1; i < size; i++){ //send boundaries to processes 1 to size
			MPI_Send((void *)&blockSize, 1, MPI_INT, i, 0xACE5, MPI_COMM_WORLD);
		}
		int primes[n-(blockSize*(size-1))];
		int j;
		for(i = 0; i < (n-(blockSize*(size-1)));i++){
			primes[i] = (blockSize*(size-1)) + i ;
		}
		for(i = 2; i < sqrt(n); i++){
			for(j = 0; j < (n-(blockSize*(size-1))); j++){
				if(primes[j] %i ==0&& i != primes[j]){
					primes[j] = 0;
				}			
			}	
		}
		int finalPrimes[n];
		int primesOther[blockSize];
		for(i = 0; i < n;i++){
			finalPrimes[i] = 0 ;
		}
		for(i = 1; i < size; i++){
			MPI_Recv(primesOther, blockSize, MPI_INT, i, 0xACE5, MPI_COMM_WORLD, &s);
			printf("asd\n");
			for(j = 0; j < blockSize;j++){
				finalPrimes[blockSize*(i-1)+j] = primesOther[j];
			}
		}
		printf("asd\n");
		for(i = 0; i < (n-(blockSize*(size-1)));i++){
			finalPrimes[(size-1)*blockSize+i] = primes[i];
		}
		for(i = 0; i < n;i++){
			if(finalPrimes[i] != 0){
				printf("%d \n", finalPrimes[i]);
			}
		}
		double end =MPI_Wtime();
		printf("Program took %.5f seconds\n",end-start);
		
	}
	else{  
		int blockSize;
		MPI_Recv((void *)&blockSize, 1, MPI_INT, 0, 0xACE5, MPI_COMM_WORLD, &s);
		int primes[blockSize];
		int i,j;
		for(i = 0; i < blockSize;i++){
			primes[i] = ((rank-1) * blockSize) +i;
		}
		for(i = 2; i < sqrt(rank*blockSize); i++){
			for(j = 0; j < blockSize; j++){
				if(primes[j] %i == 0 && i != primes[j]){
					primes[j] = 0;
				}
			}	
		}
		printf("asd\n");
		MPI_Send(primes, blockSize, MPI_INT, 0, 0xACE5, MPI_COMM_WORLD);
		}
	}

	MPI_Finalize();
	return 0;
}

