#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

/*
 * mpicc -std=c99 scatter.c -o scatter
 * mpiexec -np 4 scatter
 */

// use exactly 4 CPU's in this example or find out what happens ... :-)

int main(int argc, char* argv[]) 
{
	int procRank,procCount;

	MPI_Init(&argc,&argv);

	MPI_Comm_size(MPI_COMM_WORLD,&procCount);
	MPI_Comm_rank(MPI_COMM_WORLD,&procRank);
		
  printf("Start[%d]/[%d] \n",procRank,procCount);

  enum { tagSend = 1 };

  const int k = 20;
  const int l = k/4;
  int vector1[k], vector2[k], buffer[l];

  // init for master
  if (0==procRank)
  {  
    for (int i=0; i<k; ++i) {
      vector1[i]=i*35;   
      vector2[i]=0;   
    }
	  for (int i=0; i<k; ++i) 
      printf("vectors %i -th elements %i %i\n",i,vector1[i],vector2[i]);
  }
  
  MPI_Scatter(vector1, 5, MPI_INT, buffer, 5, MPI_INT, 0, MPI_COMM_WORLD);
  
  for (int i=0; i<l; ++i) 
    printf("Process %i has %i -th element %i\n",procRank,i,buffer[i]);

  // compute!
  for (int i=0; i<l; ++i) 
    buffer[i] -= 10;
  
  MPI_Gather(buffer,5,MPI_INT,vector2,5,MPI_INT,0,MPI_COMM_WORLD);

  if (0==procRank)
  {  
	  for (int i=0; i<k; ++i) 
      printf("vectors %i -th elements %i %i\n",i,vector1[i],vector2[i]);
  }

	MPI_Finalize();
	
	return EXIT_SUCCESS;
}

