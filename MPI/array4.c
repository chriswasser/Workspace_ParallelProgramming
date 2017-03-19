#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

/*
 * gcc -std=c99 arraydistI-serial.c -o arraydistI-serial
 * ./arraydistI-serial
 */

int countPattern(const int v[], const int size, const int pattern)
{
 	int count=0;

	for(int i=0; i<size; ++i) {
		if(pattern==v[i]) {
			++count;
		}
	}
	return count;
}

void initArray(int v[], const int size, const int mod) 
{
	for(int i=0; i<size; ++i) {
		v[i] = rand() % mod;
	}
}

void printArray(const int v[], const int size) 
{
	printf("Array:");
	for(int i=0; i<size; ++i) { 
		printf(" %i", v[i]);
	}
	printf("\n");
}

int main (int argc, char* argv[])
{
	int myRank, nProcs;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nProcs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
	
	int *vector, *buffer;
	int sum = 0, count, pattern, numpp;

	if(myRank == 0) { // only master thread has crucial information which needs to be distributed among all processes
		// data memory
		const int k = 50;
		vector = (int*) malloc(k * sizeof(int));

  		// initializing and printing the array
  		initArray(vector,k,4);
		printArray(vector, k);
  		pattern = 0;
  		numpp = k / nProcs; // assuming k is divisble by number of processes without remainder
		sum = 0;
	}

	// Bcast search pattern
	MPI_Bcast(&pattern, 1, MPI_INT, 0, MPI_COMM_WORLD);

	// Bcast numpp to be compute by each process
	MPI_Bcast(&numpp, 1, MPI_INT, 0, MPI_COMM_WORLD);

	// scatter vector into buffer
	buffer = (int*) malloc(numpp * sizeof(int));
	MPI_Scatter(vector, numpp, MPI_INT, buffer, numpp, MPI_INT, 0, MPI_COMM_WORLD);

	// compute count and reduce into sum
	count = countPattern(buffer, numpp, pattern);
	MPI_Reduce(&count, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

	if(myRank == 0) {
		// print results
		printf("Number %i was found %i times in the array\n", pattern, sum);

		// cleanup
  		free(vector);
	}

	// cleanup
	free(buffer);

	MPI_Finalize();

  	return EXIT_SUCCESS;
}
