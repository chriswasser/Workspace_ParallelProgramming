#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char* argv[]) 
{
	const int maxCount = 3;
	int nProcs, myRank, i, count;

	MPI_Init(&argc, &argv);

	MPI_Comm_size(MPI_COMM_WORLD, &nProcs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

	if(myRank == 0) {
		printf("Start [%d]/[%d]\n", myRank, nProcs);
		count = 0;
		for(i = 0; i < maxCount; ++i) {
			count++;
			printf("%d sent: Ping from proc %d\n", count, myRank);
			MPI_Send(&count, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
			MPI_Recv(&count, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		}
	} else if(myRank == 1) {
		printf("Start [%d]/[%d]\n", myRank, nProcs);
		for(i = 0; i < maxCount; ++i) {
			MPI_Recv(&count, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			count++;
			printf("%d sent: Pong from proc %d\n", count, myRank);
			MPI_Send(&count, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
		}
	}

	MPI_Finalize();

	return EXIT_SUCCESS;
}
