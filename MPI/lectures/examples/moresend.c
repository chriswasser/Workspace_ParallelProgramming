#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

/*
 * mpicc -std=c99 moresend.c -o moresend
 * mpiexec -np 4 moresend
 */

int main(int argc, char** argv) 
{
	int procRank,procCount;

	MPI_Init(&argc,&argv);

	MPI_Comm_size(MPI_COMM_WORLD,&procCount);
	MPI_Comm_rank(MPI_COMM_WORLD,&procRank);
		
  printf("Start[%d]/[%d] \n",procRank,procCount);

  const int length = 5; // change output if length!=5

  int message[length];
  enum { tagSend = 1 };
   
  if (0==procRank) // root recv.
  {
    for (int i=1; i<procCount; ++i) 
    {
      // receive from process i
      MPI_Recv(message, length, MPI_INT, i, tagSend, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

      // change this if length!=5
      printf("Message: %i %i %i %i %i\n",message[0], message[1],message[2], message[3], message[4]);
      printf("Source:  %d\n",i);
    }
  }
  else // workers send
  {
    // build message:
    //    process 1: 12345
    //    process 2: 23456
    //    process 3: 34567
    for (int i=0; i<length; ++i)
      message[i]=procRank+i;

    MPI_Send(message, length, MPI_INT, 0, tagSend, MPI_COMM_WORLD);
  }

  MPI_Finalize();

  return EXIT_SUCCESS;
}

