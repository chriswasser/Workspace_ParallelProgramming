#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // sleep
#include "mpi.h"

/*
 * mpicc -std=c99 timer.c -o timer
 * mpiexec -np 3 timer
 */

int main(int argc, char* argv[]) 
{
	int procRank,procCount;

	MPI_Init(&argc,&argv);

	MPI_Comm_size(MPI_COMM_WORLD,&procCount);
	MPI_Comm_rank(MPI_COMM_WORLD,&procRank);
		
  printf("Start[%d]/[%d] \n",procRank,procCount);

  double tm0,tm1;
  
  tm0 = MPI_Wtime();

  sleep(procRank);
  
  // comment out
  MPI_Barrier(MPI_COMM_WORLD); // all wait
  
  tm1 = MPI_Wtime();
  
  printf("proc %d, Wtime %lf \n",procRank,(tm1-tm0));
      

	MPI_Finalize();
	
	return EXIT_SUCCESS;
}
