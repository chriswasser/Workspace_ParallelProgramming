#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char* argv[]) 
{
	int myRank, nProcs;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nProcs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
	
	if(nProcs >= 20) {
		int i, keep[20];
		MPI_Group global, restricted;
		MPI_Comm restrictedComm;
		for(i = 0; i < 20; ++i) {
			keep[i] = i;
		}
		
		// can also be split with myRank / 20 and then color > 0 split key MPI_UNDEFINED
		MPI_Comm_group(MPI_COMM_WORLD, &global);
		MPI_Group_incl(global, 20, keep, &restricted);
		MPI_Comm_create(MPI_COMM_WORLD, restricted, &restrictedComm);

		if(restrictedComm != MPI_COMM_NULL) { // processes which are not part of the new communicator get dummy element
			// create communicator with 20 processes and divide them into groups of 5
			int color = myRank / 5;
			MPI_Comm groupComm;
			int newRank;
			MPI_Comm_split(restrictedComm, color, myRank, &groupComm);
			
			MPI_Comm_rank(groupComm, &newRank);	
			printf("I am Process %d and I am included in the new communicator with color %d and rank %d!\n", myRank, color, newRank);
			
			MPI_Comm_free(&restrictedComm);
			MPI_Comm_free(&groupComm);
		} else {
			printf("I am Process %d and I am excluded from the new communicator! :(\n", myRank);
		}
	} else {
		if(myRank == 0) {
			printf("Please start the program with more than 19 processes to get useful output!\n");
		}
	}
	
	MPI_Finalize();
	return EXIT_SUCCESS;
}
