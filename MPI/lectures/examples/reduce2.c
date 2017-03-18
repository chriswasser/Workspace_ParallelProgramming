#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

/*
 * mpicc -std=c99 reduce2.c -o reduce2
 * mpiexec -np 4 reduce2
 */

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
  int vector[k], buffer[l];

  // init for master
  if (0==procRank)
  {  
    for (int i=0; i<k; ++i)
      vector[i]=i+1;   
	  for (int i=0; i<k; ++i) 
      printf("vector %i -th elements %i \n",i,vector[i]);
  }
  
  MPI_Scatter(vector, 5, MPI_INT, buffer, 5, MPI_INT, 0, MPI_COMM_WORLD);
  
  // compute!
  int result = 0;
  for (int i=0; i<l; ++i) 
    result += buffer[i];
  
  printf("local sum proc %d is %d \n",procRank,result);

  // sum up  
  int sum = 0;
  MPI_Reduce(&result, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  // max
  int max = 0;
  MPI_Reduce(&result, &max, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
  
  if (0==procRank)
  {  
    printf("sum %d, max %d \n",sum,max);
  }
	
	MPI_Finalize();
	
	return EXIT_SUCCESS;
}
