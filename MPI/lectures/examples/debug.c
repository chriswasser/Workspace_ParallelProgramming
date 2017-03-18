#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

/*
 * mpicc -g debug.c -o debug
 * mpiexec -np 3 debug
 */

int main(int argc, char* argv[]) 
{
	int procRank,procCount;

	MPI_Init(&argc,&argv);
  
	MPI_Comm_size(MPI_COMM_WORLD,&procCount);
	MPI_Comm_rank(MPI_COMM_WORLD,&procRank);

  printf("Start[%d]/[%d] \n",procRank,procCount);

  if (0==procRank)
  {
    int loop = 1;
    while (loop)
      ;
  }
  MPI_Barrier(MPI_COMM_WORLD);
  
  printf("Cont [%d]/[%d] \n",procRank,procCount);

  MPI_Barrier(MPI_COMM_WORLD);
  	
	MPI_Finalize();
	
	return EXIT_SUCCESS;
}
