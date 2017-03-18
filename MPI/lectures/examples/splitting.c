#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

/*
 * mpicc -std=c99 splitting.c -o splitting
 * mpiexec -np 8 splitting
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
  
  int color = procRank%4;
  
  MPI_Comm newcomm;
  MPI_Comm_split(MPI_COMM_WORLD, color, procRank, &newcomm);

  int procRankNew,procCountNew;
   
  MPI_Comm_size(newcomm, &procCountNew);
  MPI_Comm_rank(newcomm, &procRankNew);

  if (0==procRankNew) 
    message=17+color; // procs in same new communicator get same message
  
  MPI_Bcast(&message, 1, MPI_INT, 0, newcomm);
  printf("proc %d, color %d, new rank %d has Message: %d\n",procRank,color,procRankNew,message);   

  MPI_Comm_free(&newcomm);

   	
	MPI_Finalize();
	
	return EXIT_SUCCESS;
}
