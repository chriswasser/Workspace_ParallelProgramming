#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

/*
 * mpicc -std=c99 allreduce.c -o allreduce
 * mpiexec -np 4 allreduce
 */

int main(int argc, char* argv[]) 
{
	int procRank,procCount;

	MPI_Init(&argc,&argv);

	MPI_Comm_size(MPI_COMM_WORLD,&procCount);
	MPI_Comm_rank(MPI_COMM_WORLD,&procRank);
		
  printf("Start[%d]/[%d] \n",procRank,procCount);

  int message = -1;
  enum { tagSend = 1 };

  // init for master
  if (0==procRank)
  {  
    message = 5;
  }
  
  // distribute necessary data for local computations
  MPI_Bcast(&message,1, MPI_INT, 0, MPI_COMM_WORLD);
  
  // compute local results
  int result = procRank*message;

  // sum up  
  int sum = 0;
  MPI_Allreduce(&result, &sum, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
  
  printf("proc %d: sum %d \n",procRank,sum);
	
	MPI_Finalize();
	
	return EXIT_SUCCESS;
}
