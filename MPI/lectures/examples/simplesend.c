#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

/*
 * mpicc -std=c99 simplesend.c -o simplesend
 * mpiexec -np 2 simplesend
 */

int main(int argc, char** argv) 
{
	int procRank,procCount;

	MPI_Init(&argc,&argv);

	MPI_Comm_size(MPI_COMM_WORLD,&procCount);
	MPI_Comm_rank(MPI_COMM_WORLD,&procRank);
		
  printf("Start[%d]/[%d] \n",procRank,procCount);

  int message = -1;
  enum { tagSend = 1 };
   
  printf("Message undef.: %i\n",message);

  if (0==procRank) // root defines a message and sends it to the worker
  {
    message=42;
    // remember &message returns address of message
    MPI_Send(&message, 1, MPI_INT, 1, tagSend, MPI_COMM_WORLD);
  }
  else // if (1==procRank) // only one worker and worker receives the message
  {
    // parameters must match!!!
    MPI_Recv(&message, 1, MPI_INT, 0, tagSend, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	  printf("Recv. Message: %i\n",message);
  }

  MPI_Finalize();

  return EXIT_SUCCESS;
}
