#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

/*
 * mpicc -std=c99 prim-serial.c -o prim-serial
 * mpiexec -np 1 prim-serial
 */

int isPrim(const int num) {
	int n = sqrt((double) num);
	for(int i=2;i<n;i++) {
		if(num%i==0) {
			return 0;
		}
	}
	return 1;
}

int main(int argc, char* argv[]) 
{
	int iPrim;
	printf("Start to search for prim from: "); fflush(stdout);
	scanf("%i",&iPrim);
	
	while (1) {
	    if (isPrim(iPrim)) {
	    	break;
	    }
		iPrim++;
	}

	printf("Prime found: %i\n",iPrim);
	
	return EXIT_SUCCESS;
}
