#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char* argv[]) 
{
	int nProcs, myRank, error;

	MPI_Init(&argc, &argv);

	MPI_Comm_size(MPI_COMM_WORLD, &nProcs);
	error = MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

	printf("Hello World from Process %d of %d Processes received error code %d.\n", myRank, nProcs, error);

	MPI_Finalize();

	return EXIT_SUCCESS;
}
