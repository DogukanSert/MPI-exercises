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
		int low, high;
		int i;
		int items[3];
		for(i = 1; i < size; i++){ //send boundaries to processes 1 to size
			low = (i*n)/size;
			high =  floor(((i+1)*n)/size);
			items[0] = low;
			items[1] = high;
			items[2] = n;
			MPI_Send(items, 3, MPI_INT, i, 0xACE5, MPI_COMM_WORLD);
		}
		low = (rank*n)/size;
		high =  floor(((rank+1)*n)/size);
		double sum = 0.0;
		double dx = 1.0/n;
		double x,mid;
		for(i = low; i < high; i++){
			mid = dx * (i + 0.5);
			x = 1.0 / (1.0 + mid*mid);
			sum += x;
		}
		sum = 4 * sum * dx;
		double otherSum = 0.0;
		for(i = 1; i < size; i++){
			MPI_Recv((void *)&otherSum, 1, MPI_DOUBLE, i, 0xACE5, MPI_COMM_WORLD, &s);
			sum += otherSum;
		}
		double end =MPI_Wtime();
		printf("Program took %.5f seconds\n",end-start);
		printf("pi app = %.15f\n", sum);
	}else{  
		int items[3];
		MPI_Recv(items, 3, MPI_INT, 0, 0xACE5, MPI_COMM_WORLD, &s);
		printf("low %d high %d  n %d\n", items[0], items[1], items[2]);
		double sum = 0.0;
		double dx = 1.0/items[2];
		double x,mid;
		int i;
		for(i = items[0]; i < items[1]; i++){
			mid = dx * (i + 0.5);
			x = 1.0 / (1.0 + mid*mid);
			sum += x;
		}
		sum = 4 * sum * dx;
		//printf("pi app = %.15f\n", sum);
		MPI_Send((void *)&sum, 1, MPI_DOUBLE, 0, 0xACE5, MPI_COMM_WORLD);
	}

	MPI_Finalize();
	return 0;
}
}
