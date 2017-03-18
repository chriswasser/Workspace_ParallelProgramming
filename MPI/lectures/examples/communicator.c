#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

/*
 * mpicc -std=c99 communicator.c -o communicator
 * mpiexec -np 4 communicator
 */

int main(int argc, char* argv[]) 
{
	int procRank,procCount;

	MPI_Init(&argc,&argv);

	MPI_Comm_size(MPI_COMM_WORLD,&procCount);
	MPI_Comm_rank(MPI_COMM_WORLD,&procRank);
		
  printf("Start[%d]/[%d] \n",procRank,procCount);

  int message1 = -1, message2 = -1;
  enum { tagSend = 1 };

  // init for master
  if (0==procRank)
    message1 = 42;
  
  MPI_Bcast(&message1,1, MPI_INT, 0, MPI_COMM_WORLD);
  printf("bcast1: proc %d recv. bcast %d \n",procRank,message1);


  // remove two processes  
  int loser[2]; //have to leave the world_group
  MPI_Group world_group, win_group;
  MPI_Comm win_comm;
  
  // first and last have to go
  loser[0]=0;        
  loser[1]=procCount-1;

  // return group of communicator   
  MPI_Comm_group(MPI_COMM_WORLD, &world_group);
  
  // create new group without loser
  MPI_Group_excl(world_group, 2, loser, &win_group);
  
  // create communicator (subset of group of comm)
  MPI_Comm_create(MPI_COMM_WORLD, win_group, &win_comm);

  if ((procRank!=0)&&(procRank!=(procCount-1)))
  {
    int procWinRank;
    MPI_Comm_rank(win_comm, &procWinRank);

    printf("Number %d has now winner rank: %d\n",procRank,procWinRank);

    // init for winner master
    if (0==procWinRank) 
      message2=42;
    
    MPI_Bcast(&message2, 1, MPI_INT, 0, win_comm);

    MPI_Comm_free(&win_comm);
  }

  printf("bcast2: proc %d recv. bcast %d \n",procRank,message2);

   	
	MPI_Finalize();
	
	return EXIT_SUCCESS;
}
