#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

/*
 * mpicc -std=c99 newtypes.c -o newtypes
 * mpiexec -np 2 newtypes
 */
 
// use with 2 procs.

int main(int argc, char** argv) 
{
	int procRank,procCount;

	MPI_Init(&argc,&argv);

	MPI_Comm_size(MPI_COMM_WORLD,&procCount);
	MPI_Comm_rank(MPI_COMM_WORLD,&procRank);
		
  printf("Start[%d]/[%d] \n",procRank,procCount);

  enum { 
    tagSendBuffer = 1,
  };

  MPI_Datatype cont_type, vec_type, vec2_type;
  int buffer[24];

  
  // contiguous type
  
  // make type: 24 = 8*3 
  MPI_Type_contiguous(3, MPI_INT, &cont_type);
	MPI_Type_commit(&cont_type);

	if (0==procRank)
  {
    for (int i=0; i<24; i++)
      buffer[i] = i;

    // send data with new data type to worker
    MPI_Send(buffer, 8, cont_type, 1,  tagSendBuffer, MPI_COMM_WORLD);
  }
  else
  {
    for (int i=0; i<24; i++)
      buffer[i] = -1;

    // receive data with new data type from master
    MPI_Recv(buffer, 8, cont_type, 0, tagSendBuffer, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    for (int i=0; i<24; i++)
      printf("buffer[%d] = %d\n", i, buffer[i]);
  }

  
  // vector type of basic type
  
	MPI_Type_vector(3, 6, 9, MPI_INT, &vec_type);   // 3 blocks of length 6 with stride 9 -> miss elemets 6,7,8 and 15,16,17
  MPI_Type_commit(&vec_type);

	if (0==procRank)
  {
    for (int i=0; i<24; i++)
      buffer[i] = 2*i;

    // send data with new data type to worker
    MPI_Send(buffer, 1, vec_type, 1,  tagSendBuffer, MPI_COMM_WORLD);
  }
  else
  {
    for (int i=0; i<24; i++)
      buffer[i] = -1;
    
    // receive data with new data type from master
    MPI_Recv(buffer, 1, vec_type, 0, tagSendBuffer, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    for (int i=0; i<24; i++)
      printf("buffer[%d] = %d\n", i, buffer[i]);
  }

  
  // vector type of user-def. data type

  MPI_Type_vector(3, 2, 3, cont_type, &vec2_type);  // 3 blocks of cont. data of length 2 (=6 old data) with stride 3 (=9 old data)
  MPI_Type_commit(&vec2_type);

	if (0==procRank)
  {
    for (int i=0; i<24; i++)
      buffer[i] = 3*i;
    
    // send data with new data type to worker
    MPI_Send(buffer, 1, vec2_type, 1,  tagSendBuffer, MPI_COMM_WORLD);
  }
  else
  {
    for (int i=0; i<24; i++)
      buffer[i] = -1;
    
    // receive data with new data type from master
    MPI_Recv(buffer, 1, vec2_type, 0, tagSendBuffer, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    for (int i=0; i<24; i++)
      printf("buffer[%d] = %d\n", i, buffer[i]);
  }

  MPI_Type_free(&cont_type);
  MPI_Type_free(&vec_type);
  MPI_Type_free(&vec2_type);
 
  
  MPI_Finalize();

  return EXIT_SUCCESS;
}
