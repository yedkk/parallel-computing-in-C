#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
// complex algorithm for evaluation
//author: Kangdong Yuan

void transpose(double *A, double *B, int n) {
	omp_set_num_threads(10);
    int i,j;
    #pragma omp parallel for private(i,j)
	for(i=0; i<n; i++) {
		for(j=0; j<n; j++) {
			B[j*n+i] = A[i*n+j];
		}
	}
}

void matrix_mult(double *A, double *B, double *C, int N) 
{	    
	omp_set_num_threads(10);
	double *D;
	D = (double*)malloc(sizeof(double)*N*N);
    transpose(B,D, N);
	#pragma omp parallel
	{
		int i, j, k;
		#pragma omp for
		for (i = 0; i < N; i++) { 
			for (j = 0; j < N; j++) {
				double dot  = 0;
				for (k = 0; k < N; k++) {
					dot += A[i*N+k]*D[j*N+k];
				} 
				C[i*N+j] = dot;
			}
		}
		
	}
	free(D);
}

/**
void matrix_mult(double *A, double *B, double *C, int N)
{	
	#pragma omp parallel for shared(A,B,C) schedule(static)
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
		{
			C[i * N + j] = 0;
				for (int k = 0; k < N; k++)
				{
					C[i * N + j] += A[i * N + k] * B[k * N + j];
				}
		}
}
**/

