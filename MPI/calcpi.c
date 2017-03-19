#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

double f(double x) 
{
	return 1.0 / (1.0 + x*x);
}

int main(int argc, char* argv[])
{
	int myRank, nProcs;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nProcs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
	
	int n = 10000;
	int npp = n / nProcs; // assuming n is devisible by nProcs without remainder

	double piNum = 0.0;
	double piSum = 0.0;

	double a=0.0, b=1.0;
	double h = (b-a)/n;
  
	for (int i=myRank*npp; i<(myRank+1)*npp; ++i) {
		piNum += f(a+(i+0.5)*h);
	}
	
	MPI_Reduce(&piNum, &piSum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	
	if(myRank == 0) {
		piSum *= (4*h);
		printf("Pi = %10.8lf \n",piSum);
	}
	
	MPI_Finalize();

	return EXIT_SUCCESS;
}
