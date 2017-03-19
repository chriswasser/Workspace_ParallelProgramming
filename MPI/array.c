#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

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
	
	if(myRank == 0) {
		// data memory
		const int k = 50;
		int* vector = (int*) malloc(k * sizeof(int));
  		
  		// initializing and printing the array
  		initArray(vector,k,4);
  		
  		// send data vector
  		MPI_Send(vector, k, MPI_INT, 1, 0, MPI_COMM_WORLD);
  		
  		// cleanup
  		free(vector);
  	} else if(myRank == 1) {
		// get number of elements contained in the vector
  		int numElements;
  		MPI_Status status;
  		MPI_Probe(0, 0, MPI_COMM_WORLD, &status);
  		MPI_Get_count(&status, MPI_INT, &numElements);
  		int* vector = (int*) malloc(numElements * sizeof(int));
  		
  		// receive data to compute and print array
		MPI_Recv(vector, numElements, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		printArray(vector, numElements);
		
		// search pattern and counting
  		const int pattern = 0;
		int count = countPattern(vector, numElements, pattern);
		
		// output results
  		printf("Number %i was found %i times in the array\n", pattern, count);
  		
  		// cleanup
  		free(vector);
  	}

	MPI_Finalize();

  	return EXIT_SUCCESS;
}
