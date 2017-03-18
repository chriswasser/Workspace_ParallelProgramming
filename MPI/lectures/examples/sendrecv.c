#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

/*
 * mpicc -std=c99 sendrecv.c -o sendrecv
 * mpiexec -np 2 sendrecv
 */
 
// use with 2 procs.

int main(int argc, char** argv) 
{
	int procRank,procCount;

	MPI_Init(&argc,&argv);

	MPI_Comm_size(MPI_COMM_WORLD,&procCount);
	MPI_Comm_rank(MPI_COMM_WORLD,&procRank);
		
  printf("Start[%d]/[%d] \n",procRank,procCount);

  int messageMaster = -1, messageWorker = -1;
  enum { 
    tagSendMaster = 1,
    tagSendWorker
  };
   
  if (0==procRank)
  {
    messageMaster = 42;
    MPI_Sendrecv(
        &messageMaster, 1, MPI_INT, 1, tagSendMaster, 
        &messageWorker, 1, MPI_INT, 1, tagSendWorker, 
        MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  }
  else
  {
    messageWorker = 43;
    MPI_Sendrecv(
        &messageWorker, 1, MPI_INT, 0, tagSendWorker, 
        &messageMaster, 1, MPI_INT, 0, tagSendMaster, 
        MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  }
  
  printf("proc %d, master %d, worker %d \n",procRank,messageMaster,messageWorker);

  MPI_Finalize();

  return EXIT_SUCCESS;
}
