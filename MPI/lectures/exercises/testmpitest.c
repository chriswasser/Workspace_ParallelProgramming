#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <unistd.h>

int main(int argc, char* argv[]) 
{
	int myRank, nProcs;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nProcs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
	
	int message1 = -1;
	int message2 = -1;
	MPI_Request req;

	MPI_Barrier(MPI_COMM_WORLD);
	if(myRank == 0) {
		message1 = 10;
		message2 = 100;
		int flag = 0;
		long i = 0;
		double time;
		MPI_Send(&message1, 1, MPI_INT, 1, 100, MPI_COMM_WORLD); // starts time measurements
		// send message in non blocking
		time = MPI_Wtime();
		MPI_Issend(&message2, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &req); // receives message after 1 sec
		// need synchronous send otherwise data is just put into buffer and flag is set immediately
		while(flag != 1) {
			MPI_Test(&req, &flag, MPI_STATUS_IGNORE);
			i++;
		}
		 time = MPI_Wtime() - time;
		printf("Process %d executed MPI_Test %d times during 1 sec\n", myRank, i);
		printf("Exact time in while loop including MPI_Issend call: %f sec\n", time);
		// result: 19 million times
	} else if(myRank == 1) {
		MPI_Recv(&message1, 1, MPI_INT, 0, 100, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // start sleeping
		sleep(1);
		MPI_Recv(&message2, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // accept message to stop counting 
		printf("message1 = %d\n", message1);
		printf("message2 = %d\n", message2);
	}
	
	MPI_Finalize();
	return EXIT_SUCCESS;
}
