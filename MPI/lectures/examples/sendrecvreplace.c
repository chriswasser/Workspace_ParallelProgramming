#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

/*
 * mpicc -std=c99 sendrecvreplace.c -o sendrecvreplace
 * mpiexec -np 2 sendrecvreplace
 */
 
// use with 2 procs.

int main(int argc, char** argv) 
{
	int procRank,procCount;

	MPI_Init(&argc,&argv);

	MPI_Comm_size(MPI_COMM_WORLD,&procCount);
	MPI_Comm_rank(MPI_COMM_WORLD,&procRank);
		
  printf("Start[%d]/[%d] \n",procRank,procCount);

  int message = -1;
  enum { 
    tagSendMaster = 1,
    tagSendWorker
  };
   
  if (0==procRank)
  {
    message = 42;
    MPI_Sendrecv_replace(
        &message, 1, MPI_INT, 1, tagSendMaster, 
        1, tagSendWorker, 
        MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  }
  else
  {
    message = 43;
    MPI_Sendrecv_replace(
        &message, 1, MPI_INT, 0, tagSendWorker, 
        0, tagSendMaster, 
        MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  }
  
  printf("proc %d, message %d \n",procRank,message);

  MPI_Finalize();

  return EXIT_SUCCESS;
}
