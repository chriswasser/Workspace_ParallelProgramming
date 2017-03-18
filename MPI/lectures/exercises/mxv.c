#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void printMatrix(double* A, int height, int width, int info) 
{
	printf("from proc %d\n",info);
	for (int i=0; i<height; ++i) {
		printf("[ ");
		for (int j=0; j<width; ++j) {
			printf("%lf ",A[i*width+j]);
		}
		printf("]\n");
	}
}

void initFields(double* A, double* x, double* b, int height, int width)
{
	for (int i=0; i<height; ++i) {
		for (int j=0; j<width; ++j) {
			A[i*width+j] = (j+i)%width;
		}
	}
	for (int j=0; j<width; ++j) {
		x[j] = j;
	}
	for (int i=0; i<height; ++i) {
		b[i] = 0.0;
	}
}

int main (int argc, char* argv[])
{
	int myRank, nProcs;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nProcs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
	
	int width, height;
	double *A, *x, *b;
	
	if(myRank == 0) {
		width = 10;
		height = 4*4;

		// A*x = b
		A = (double*) malloc(width * height * sizeof(double));
		x = (double*) malloc(width * sizeof(double));
		b = (double*) malloc(height * sizeof(double));

		initFields(A,x,b,height,width);  
		printf("matrix A ");
		printMatrix(A,height,width,0);
		printf("vector x ");
		printMatrix(x,1,width,0);
		
		height = height / nProcs;
	}
	
	// Bcast width, height of vector and matrix
	MPI_Bcast(&height, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&width, 1, MPI_INT, 0, MPI_COMM_WORLD);
	
	double *vec, *mat, *result;
	vec = (double*) malloc(width * sizeof(double));
	mat = (double*) malloc(width * height * sizeof(double));
	result = (double*) malloc(height * sizeof(double));
	
	if(myRank == 0) {
		double* temp = vec;
		vec = x;
		x = temp;
	}
	
	// Bcast x
	MPI_Bcast(vec, width, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	// Scatter A
	MPI_Scatter(A, width*height, MPI_DOUBLE, mat, width*height, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	
	// each process executes mxv for their own part of the matrix
	// original matrix vector mult.
	for (int i=0; i<height; ++i) {
		result[i]=0.0;
		for (int j=0; j<width; ++j) {
			result[i] += mat[i*width+j]*vec[j];
		}
	}

	// Gather result into b
	MPI_Gather(result, height, MPI_DOUBLE, b, height, MPI_DOUBLE,  0, MPI_COMM_WORLD);
	
	if(myRank == 0) {
		printf("result b ");
		printMatrix(b,1,height*nProcs,0);

		// cleanup
		free(A);
		free(x);
		free(b);
	}
	
	// cleanup
	free(vec);
	free(mat);
	free(result);

	MPI_Finalize();
	
	return EXIT_SUCCESS;
}
