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
	
	int numElements, pattern;
	int* vector;

	if(myRank == 0) {
		// data memory
		const int k = 50;
		vector = (int*) malloc(k * sizeof(int));
		
		// initializing and printing the array
		initArray(vector, k, 4);
		printArray(vector, k);
		
		// set variables for uniform code
		numElements = k;
		pattern = myRank;
		
		// send information to all processes
		for(int i = 1; i < nProcs; ++i) {
			MPI_Send(vector, k, MPI_INT, i, 0, MPI_COMM_WORLD);
			MPI_Send(&i, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
		}
  	} else {
  		// get information about incoming message to allocate memory accordingly
  		MPI_Status status;
  		MPI_Probe(0, 0, MPI_COMM_WORLD, &status);
  		MPI_Get_count(&status, MPI_INT, &numElements);
  		vector = (int*) malloc(numElements * sizeof(int));
  		
  		// receive real data for counting
		MPI_Recv(vector, numElements, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		// receive count pattern
		MPI_Recv(&pattern, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  	}
	
	// computation done by all processes
	int count = countPattern(vector, numElements, pattern);
	MPI_Send(&count, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);

	// collect information on master
	if(myRank == 0) {	
		for(int i = 0; i < nProcs; ++i) {
			MPI_Recv(&count, 1, MPI_INT, i, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			printf("Number %i was found %i times in the array\n", i, count);
		}
	}
	
	free(vector);
	MPI_Finalize();

  	return EXIT_SUCCESS;
}
