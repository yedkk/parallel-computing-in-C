
#include <stdlib.h>
#include <thread>

using namespace std;
//author: kangdong Yuan
#define NUMTHREADS 20
struct MYPARAM{
	int i_start;
	int i_stop;
};
std::thread t[NUMTHREADS];

// create an array of thread parameters (one for each thread)
struct MYPARAM *p_params = new struct MYPARAM[NUMTHREADS];
void run_test_parallel(struct MYPARAM *p_params, double *A,double *B,double *C,int N)
{
    // Go over every column of matrixB
	for(int k = p_params->i_start; k < p_params->i_stop; k++)
    for (int i = 0; i<N; i++)
    {
        // to compute every element of (row_index)-th row of answer
      //  C[row_index][i] = 0;
		C[k*N+i]=0;
        // Compute the answer
        // Number of columns in A = Number of rows in B
        for (int j = 0; j<N; j++)
         //   Mat[2][row_index][i] += Mat[0][row_index][j] * Mat[1][j][i];
			C[k*N+i]+=A[k*N+j]*B[j*N+i];
    }
}
 void matrix_mult(double *A, double *B, double *C, int N){
	for (int i = 0; i < NUMTHREADS; i++)
	{
		p_params[i].i_start = i * (N/NUMTHREADS);
		p_params[i].i_stop = (i + 1) * (N/NUMTHREADS);
	}
	for (int i = 0; i < NUMTHREADS; i++)
	{
		t[i] = std::thread(run_test_parallel, &p_params[i],A,B,C,N);
	}
    for (int i = 0; i<NUMTHREADS; i++)
        t[i].join();
 }
