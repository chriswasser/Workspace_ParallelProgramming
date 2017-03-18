#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

/*
 * mpicc -std=c99 probe.c -o probe
 * mpiexec -np 4 probe
 */

int main(int argc, char** argv) 
{
	int procRank,procCount;

	MPI_Init(&argc,&argv);

	MPI_Comm_size(MPI_COMM_WORLD,&procCount);
	MPI_Comm_rank(MPI_COMM_WORLD,&procRank);
		
  printf("Start[%d]/[%d] \n",procRank,procCount);

  enum { tagSend = 1 };
   
  if (0==procRank) // root defines a message and sends it to all workers
  {
    const int k=3;
    int* message = (int*)malloc(k*sizeof(int));
    message[0] = message[1] = message[2] = 42;

    // !!! message is still a pointer to the buffer !!!
    for (int i=1; i<procCount; ++i)
      MPI_Send(message, k, MPI_INT, i, tagSend, MPI_COMM_WORLD);
    
    free(message);
  }
  else
  {
    MPI_Status status;
    MPI_Probe(0, tagSend, MPI_COMM_WORLD, &status);
    int k=3;
    MPI_Get_count(&status,MPI_INT, &k);

    int* message = (int*)malloc(k*sizeof(int));

    // !!! message is still a pointer to the buffer !!!
    MPI_Recv(message, k, MPI_INT, 0, tagSend, MPI_COMM_WORLD, &status);
	  printf("Recv. Message, proc %d, length %d: ... %d ...\n",procRank,k,message[2]);

    free(message);
  }

  MPI_Finalize();

  return EXIT_SUCCESS;
}
