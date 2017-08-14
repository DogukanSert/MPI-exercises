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
	double sum = 0.0;
	double dx = 1.0/n;
	double x,mid;
	int i;
	for(i = low; i < high; i++){
		mid = dx * (i + 0.5);
		x = 1.0 / (1.0 + mid*mid);
		sum += x;
	}
	sum = 4 * sum * dx;
	double finalSum = 0.0;
	MPI_Allreduce((void *)&sum,(void *)&finalSum,1,MPI_DOUBLE,MPI_SUM,MPI_COMM_WORLD);
	if(rank == 0){
		double end =MPI_Wtime();
		printf("Program took %.5f seconds\n",end-start);
		printf("pi app = %.15f\n", finalSum);
	}
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Finalize();
	return 0;
}
}
