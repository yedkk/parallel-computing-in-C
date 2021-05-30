#include <iostream>
#include <sys/time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "sd_thread.h"
#include <omp.h>

using namespace std;
//author: Kangdong Yuan



STDDEV_RESULT* calcSdThread(double *A, long N, int P)
{
    struct STDDEV_RESULT* res = new STDDEV_RESULT;
    omp_set_num_threads(P);
    double sd_temp, mean, mymin, mymax, sd;
    
    mymin = RAND_MAX;
    mymax = 0.0;
    sd = 0;
    sd_temp = 0;
    mean = 0;

	// perform the summation for the mean
	#pragma omp parallel for shared(A) reduction(+:mean)
	for(long i = 0; i < N; i++)
	{
		mean = mean+A[i];
	}

	mean /= (double) N;

	// perform the summation for the std_dev
	#pragma omp parallel for shared(A) reduction(+:sd_temp)
	for(long i = 0; i < N; i++)
	{
		sd_temp += (A[i] - mean) * (A[i] - mean);
	}	
	sd=sqrt(sd_temp/(double)N);
	
	// find min and max

	#pragma omp parallel for shared(A) reduction(max:mymax) reduction(min:mymin)
	for(long i = 0; i < N; i++)
	{	
		if(mymax < A[i])
		{
			mymax = A[i];
		}
		if(mymin > A[i])
		{
			mymin = A[i];
		}
	}
	
	// store off the values to return 
	res->mean = mean;
	res->min = mymin;
	res->max = mymax;
	res->stddev = sd;
	
    return res;
}

THRESH_RESULT *findThreshValuesThread(double *A, long N, double T, int P)
{
	THRESH_RESULT *p_tmpResult = new THRESH_RESULT;
	omp_set_num_threads(P);

	
	// traverse the list once to find the count of values over threshold
	long c = 0;
	#pragma omp parallel for shared(A) reduction(+:c)
	for (long i=0; i < N; i++)
	{
		if (A[i] > T)
			c++;
	}
	
	// store the count and allocate an array to store the results
	p_tmpResult->li_threshCount = c;
	p_tmpResult->pli_list = new long[c];
	c = 0;
	
	// store the index locations of the values over threshold
	#pragma omp parallel for shared(A) 
	for (long i=0; i < N; i++){
		if (A[i] > T){
			#pragma omp critical
			{
			p_tmpResult->pli_list[c] = i;
			c++;
			}
		}
	}
	
	return p_tmpResult;
}
