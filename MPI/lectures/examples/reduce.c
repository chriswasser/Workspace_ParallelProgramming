#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

/*
 * mpicc -std=c99 reduce.c -o reduce
 * mpiexec -np 4 reduce
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
  MPI_Reduce(&result, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
  
  if (0==procRank)
  {  
    printf("sum %d \n",sum);
  }
	
	MPI_Finalize();
	
	return EXIT_SUCCESS;
}
