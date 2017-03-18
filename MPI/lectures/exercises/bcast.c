#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char* argv[]) 
{
	const int maxCount = 100;
	int nProcs, myRank, i, j, message = -1;

	MPI_Init(&argc, &argv);

	MPI_Comm_size(MPI_COMM_WORLD, &nProcs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
	printf("Start [%d]/[%d]\n", myRank, nProcs);

	if(myRank == 0) {
		double wholeTime = 0.0;
		double time;
		int temp;
		for(i = 0; i < maxCount; ++i) {
			message = 0;
			time = MPI_Wtime();
			for(j = 1; j < nProcs; ++j) {
				MPI_Recv(&temp, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				message += temp;
			}
			time = MPI_Wtime() - time;
			wholeTime += time;
		}
		printf("Result Message on Master: %d\n", message);
		printf("Total Execution Time: %f\n", wholeTime);
		printf("Average Executing Time for one bcast communication with %d Processes: %f\n", nProcs, wholeTime/maxCount);
	} else if(myRank == 1) {
		message = 10;
		for(i = 0; i < maxCount; ++i) {
			MPI_Send(&message, 1, MPI_INT, myRank*2, 0, MPI_COMM_WORLD);
			MPI_Send(&message, 1, MPI_INT, myRank*2+1, 0, MPI_COMM_WORLD);
			MPI_Send(&message, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
		}
	} else {
		for(i = 0; i < maxCount; ++i) {
			MPI_Recv(&message, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			if(myRank*2 < nProcs) {
				MPI_Send(&message, 1, MPI_INT, myRank*2, 0, MPI_COMM_WORLD);
			}
			if(myRank*2+1 < nProcs) {
				MPI_Send(&message, 1, MPI_INT, myRank*2+1, 0, MPI_COMM_WORLD);			
			}
			MPI_Send(&message, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
		}
	}
	

	MPI_Finalize();

	return EXIT_SUCCESS;
}
