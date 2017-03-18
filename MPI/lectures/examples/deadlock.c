#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

/*
 * mpicc -std=c99 deadlock.c -o deadlock
 * mpiexec -np 2 deadlock
 */

// use exactly two processes

int main(int argc, char** argv) 
{
	int procRank,procCount;

	MPI_Init(&argc,&argv);

	MPI_Comm_size(MPI_COMM_WORLD,&procCount);
	MPI_Comm_rank(MPI_COMM_WORLD,&procRank);
		
  printf("Start[%d]/[%d] \n",procRank,procCount);

  int messageS = 42+procRank;
  int messageR = -1;
  enum { tagSend = 1 };

  // force sync. send, wait for rcv. in any case
  // MPI_Send(&messageS, 1, MPI_INT, 1-procRank, tagSend, MPI_COMM_WORLD);
  MPI_Ssend(&messageS, 1, MPI_INT, 1-procRank, tagSend, MPI_COMM_WORLD);
  MPI_Recv(&messageR, 1, MPI_INT, 1-procRank, tagSend, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

  printf("proc %d finished, message %d \n",procRank,messageR);
  
  MPI_Finalize();

  return EXIT_SUCCESS;
}

