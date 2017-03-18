#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <mpi.h>

int isPrime(unsigned long long num) {
	unsigned long long n = sqrt((double) num);
	for(unsigned long long i=2;i<n;i++) {
		if(num%i==0) {
			return 0;
		}
	}
	return 1;
}

int main(int argc, char* argv[]) 
{
	int myRank, nProcs;

	MPI_Init(&argc, &argv);

	MPI_Comm_size(MPI_COMM_WORLD, &nProcs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
	
	int tagNotFound = 0; // number was no prime
	int tagFound = 1; // number was prime
	int tagWork = 2; // work for workers
	int tagStop = 3; // prime has been found --> stop working

	unsigned long long prime;
	MPI_Status status;
	MPI_Request req;
	
	if(myRank == 0) { // master
		// variable declaration
		unsigned long long result;
		MPI_Status status2;
		double time;
		int i;
		unsigned long long input;
		
		// get user input
		printf("Start to search for prime from: "); fflush(stdout);
		scanf("%llu",&input);
		prime = input;
		
		// time measurement start
		time = MPI_Wtime();
		
		// distribute first workload to all processes
		for(i = 1; i < nProcs; ++i) {
			MPI_Issend(&prime, 1, MPI_UNSIGNED_LONG_LONG, i, tagWork, MPI_COMM_WORLD, &req);
			prime++;
		}
		
		// break if prime was found
		while(1) {
			
			// get result from any worker
			MPI_Recv(&result, 1, MPI_UNSIGNED_LONG_LONG, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			
			if(status.MPI_TAG == tagFound) {
				
				// make sure result is smallest prime by receiving all other results as well
				for(i = 1; i < nProcs; ++i) {
					if(status.MPI_SOURCE != i) {
						MPI_Recv(&prime, 1, MPI_UNSIGNED_LONG_LONG, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status2);
						if(status2.MPI_TAG == tagFound && result > prime) {
							result = prime;
						}
					}
					// notify all processes to stop working
					MPI_Issend(&result, 1, MPI_UNSIGNED_LONG_LONG, i, tagStop, MPI_COMM_WORLD, &req);
				}
				
				break;
			}
			
			// send new work to worker and increment prime
			MPI_Issend(&prime, 1, MPI_UNSIGNED_LONG_LONG, status.MPI_SOURCE, tagWork, MPI_COMM_WORLD, &req);
			prime++;
		}
		
		// time measurement end
		time = MPI_Wtime() - time;
		
		// result output
		printf("Prime found: %llu\n", result);
		printf("Gap input number to next prime: %llu\n", (result - input));
		printf("Calculation took: %f seconds\n", time);
	} else { // worker
		
		// break if master notifies to stop working
		while(1) {
		
			// receive work from master
			MPI_Recv(&prime, 1, MPI_UNSIGNED_LONG_LONG, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			
			if(status.MPI_TAG == tagStop) {
				break;
			} else {
			
				// check if prime is indeed a prime and send result accordingly
				if(isPrime(prime)) {
					MPI_Issend(&prime, 1, MPI_UNSIGNED_LONG_LONG, 0, tagFound, MPI_COMM_WORLD, &req);
				} else {
					MPI_Issend(&prime, 1, MPI_UNSIGNED_LONG_LONG, 0, tagNotFound, MPI_COMM_WORLD, &req);
				}
			}
		}
	}

	MPI_Finalize();
	
	return EXIT_SUCCESS;
}
