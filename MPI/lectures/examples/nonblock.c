#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "mpi.h"

/*
 * mpicc -std=c99 nonblock.c -o nonblock
 * mpiexec -np 2 nonblock
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
  
  double t0,t1;

  if (0==procRank)
  {
    message=42;
    printf("0: start Bsend \n");
    t0=MPI_Wtime();
    MPI_Bsend(&message, 1, MPI_INT, 1, tagSend, MPI_COMM_WORLD);
    t1=MPI_Wtime();
    printf("0: finished Bsend %lf \n",(t1-t0));
  }
  else if (1==procRank)
  {
    sleep(3);
    MPI_Recv(&message, 1, MPI_INT, 0, tagSend, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	  printf("1: recv. Bsend message: %i\n",message);
  }

  printf("proc %d: barrier Bsend \n",procRank);
  MPI_Barrier(MPI_COMM_WORLD);
  
  if (0==procRank)
  {
    message=42;
    printf("0: start Ssend \n");
    t0=MPI_Wtime();
    MPI_Ssend(&message, 1, MPI_INT, 1, tagSend, MPI_COMM_WORLD);
    t1=MPI_Wtime();
    printf("0: finished Ssend %lf \n",(t1-t0));
  }
  else if (1==procRank)
  {
    sleep(3);
    MPI_Recv(&message, 1, MPI_INT, 0, tagSend, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	  printf("1: recv. Ssend message: %i\n",message);
  }

  printf("proc %d: barrier Ssend \n",procRank);
  MPI_Barrier(MPI_COMM_WORLD);

  if (0==procRank)
  {
    message=42;
    printf("0: start Issend \n");
    t0=MPI_Wtime();
    MPI_Request req;
    MPI_Issend(&message, 1, MPI_INT, 1, tagSend, MPI_COMM_WORLD, &req);
    t1=MPI_Wtime();
    printf("0: finished Issend %lf \n",(t1-t0));
    int flag=0;
    while (0==flag) {
      MPI_Test(&req,&flag,MPI_STATUS_IGNORE);
    }
    t1=MPI_Wtime();
    printf("0: finished Issend, req done %lf \n",(t1-t0));
    
  }
  else if (1==procRank)
  {
    sleep(3);
    MPI_Recv(&message, 1, MPI_INT, 0, tagSend, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	  printf("1: recv. Issend message: %i\n",message);
  }

  printf("proc %d: barrier Issend \n",procRank);
  MPI_Barrier(MPI_COMM_WORLD);

  MPI_Finalize();

  return EXIT_SUCCESS;
}
