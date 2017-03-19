#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char* argv[]) 
{
	const int maxCount = 100;
	int nProcs, myRank, i, count;

	MPI_Init(&argc, &argv);

	MPI_Comm_size(MPI_COMM_WORLD, &nProcs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

#ifdef VERBOSE
	printf("Start [%d]/[%d]\n", myRank, nProcs);
#endif

	if(myRank == 0) {
		count = 0;
		double wholeTime = 0.0;
		double time;
		for(i = 0; i < maxCount; ++i) {
			count++;
		#ifdef VERBOSE
			printf("%d sent: Message from proc %d\n", count, myRank);
		#endif
			time = MPI_Wtime();
			MPI_Send(&count, 1, MPI_INT, (myRank+1)%nProcs, 0, MPI_COMM_WORLD);
			MPI_Recv(&count, 1, MPI_INT, (myRank-1)%nProcs, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			time = MPI_Wtime() - time;
			wholeTime += time;
		}	
	#ifdef VERBOSE
		usleep(1000); // to be sure time measurement results are displayed as the last lines
	#endif
		printf("Total Execution Time: %f\n", wholeTime);
		printf("Average Executing Time for one circular communication with %d Processes: %f\n", nProcs, wholeTime/maxCount);
	} else {
		for(i = 0; i < maxCount; ++i) {
			MPI_Recv(&count, 1, MPI_INT, (myRank-1)%nProcs, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			count++;
		#ifdef VERBOSE
			printf("%d sent: Message from proc %d\n", count, myRank);
		#endif
			MPI_Send(&count, 1, MPI_INT, (myRank+1)%nProcs, 0, MPI_COMM_WORLD);
		}
	}

	MPI_Finalize();

	return EXIT_SUCCESS;
}
