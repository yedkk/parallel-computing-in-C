#include <vector>
#include <iostream>
#include <sys/time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include "hw3_part1.h"

using namespace std;
//author: Kangdong Yuan
#define NUMTHREADS 40
struct MYPARAM{
	int i_start;
	int i_stop;
	double d_step;
	double d_result;
	double sd_temp, mean, min, max, sd;
	long c;
	long *pli_list;
	};
	
void calMean(struct MYPARAM *p_params,double *A){
	for (int i = p_params->i_start; i < p_params->i_stop; i++)
    {
        p_params->mean+=A[i];
    }
}
void calSdMinMax(struct MYPARAM *p_params,double *A,double mean){
	for (int i = p_params->i_start; i < p_params->i_stop; i++)
    {
        p_params->sd_temp+=(A[i] - mean) * (A[i] - mean);
		if(p_params->max < A[i])
		{
			p_params->max = A[i];
		}
		if(p_params->min > A[i])
		{
			p_params->min = A[i];
		}
    }
	 
}

// void calMinMax(struct MYPARAM *p_params,double *A,double mean){
// 	// find min and max
// 	for(int i = p_params->i_start; i < p_params->i_stop; i++)
// 	{
// 		if(p_params->max < A[i])
// 		{
// 			p_params->max = A[i];
// 		}
// 		if(p_params->min > A[i])
// 		{
// 			p_params->min = A[i];
// 		}
// 	}
// }




	
STDDEV_RESULT* calcSdThread(double *A, long N, int P)
{	std::vector<std::thread>t;
	t.resize(P);
	// create an array of thread parameters (one for each thread)
	struct MYPARAM *p_params = new struct MYPARAM[P];
    // setup thread parameter structures
	for (int i = 0; i < P; i++)
	{
		p_params[i].i_start = i * (N/P);
		p_params[i].i_stop = (i + 1) * (N/P);
		p_params[i].sd_temp = 0;
		p_params[i].mean = 0;
		p_params[i].min = RAND_MAX;
		p_params[i].max = 0.0;
		p_params[i].sd = 0;
		p_params[i].c = 0;
		
	}
	struct STDDEV_RESULT* res = new STDDEV_RESULT;
    
    double sd_temp, mean, min, max, sd;
    
    min = RAND_MAX;
    max = 0.0;
    sd = 0;
    sd_temp = 0;
    mean = 0;

	// perform the summation for the mean
	// for(long i = 0; i < N; i++)
	// {
	// 	mean = mean+A[i];
	// }
	for (int i = 0; i < P; i++)
	{
		t[i] = std::thread(calMean, &p_params[i],A);
	}
	
	// join the threads
	for (int i = 0; i < P; i++)
		t[i].join();

	// collect the results
	for (int i = 0; i < P; i++)
		mean += p_params[i].mean;

	mean /= (double) N;

	// perform the summation for the std_dev
	// for(long i = 0; i < N; i++)
	// {
	// 	sd_temp += (A[i] - mean) * (A[i] - mean);
	// }	
	for (int i = 0; i < P; i++)
	{
		t[i] = std::thread(calSdMinMax, &p_params[i],A,mean);
	}
	
	// join the threads
	for (int i = 0; i < P; i++)
		t[i].join();

	// collect the results
	for (int i = 0; i < P; i++){
		sd_temp += p_params[i].sd_temp;
		if(max < p_params[i].max)
		{
			max = p_params[i].max;
		}
		if(min >  p_params[i].min)
		{
			min = p_params[i].min;
		}
	}
	sd=sqrt(sd_temp/(double)N);
	
	// // find min and max
	// for(long i = 0; i < N; i++)
	// {
	// 	if(max < A[i])
	// 	{
	// 		max = A[i];
	// 	}
	// 	if(min > A[i])
	// 	{
	// 		min = A[i];
	// 	}
	// }
	
	// store off the values to return 
	res->mean = mean;
	res->min = min;
	res->max = max;
	res->stddev = sd;
	
    return res;
}

void calThreshc(struct MYPARAM *p_params,double *A,double T){
	for (int i = p_params->i_start; i < p_params->i_stop; i++)
	{
		if (A[i] > T)
			p_params->c++;
	}
	p_params->pli_list = new long[p_params->c];
}
void calThreshIndex(struct MYPARAM *p_params,double *A,double T){
	
	long c=0;
	for (int i = p_params->i_start; i < p_params->i_stop; i++){
		if (A[i] > T){
			p_params->pli_list[c] = i;
			c++;
		}
	}
}

THRESH_RESULT *findThreshValuesThread(double *A, long N, double T, int P)
{	
	THRESH_RESULT *p_tmpResult = new THRESH_RESULT;
	std::vector<std::thread>t;
	t.resize(P);
	struct MYPARAM *p_params = new struct MYPARAM[P];
    // setup thread parameter structures
	for (int i = 0; i < P; i++)
	{
		p_params[i].i_start = i * (N/P);
		p_params[i].i_stop = (i + 1) * (N/P);
		p_params[i].sd_temp = 0;
		p_params[i].mean = 0;
		p_params[i].min = RAND_MAX;
		p_params[i].max = 0.0;
		p_params[i].sd = 0;
		p_params[i].c = 0;
		
	}
	
	// traverse the list once to find the count of values over threshold
	long c = 0;
	// for (long i=0; i < N; i++)
	// {
	// 	if (A[i] > T)
	// 		c++;
	// }
	for (int i = 0; i < P; i++)
	{
		t[i] = std::thread(calThreshc, &p_params[i],A,T);
	}
	
	// join the threads
	for (int i = 0; i < P; i++)
		t[i].join();

	// collect the results
	for (int i = 0; i < P; i++)
		c += p_params[i].c;
	
	// store the count and allocate an array to store the results
	p_tmpResult->li_threshCount = c;
	p_tmpResult->pli_list = new long[c];
	c = 0;
	
	// store the index locations of the values over threshold
	// for (long i=0; i < N; i++){
	// 	if (A[i] > T){
	// 		p_tmpResult->pli_list[c] = i;
	// 		c++;
	// 	}
	// }
	for (int i = 0; i < P; i++)
	{
		t[i] = std::thread(calThreshIndex, &p_params[i],A,T);
	}
	
	// join the threads
	for (int i = 0; i < P; i++)
		t[i].join();

	// collect the results

	for (int i = 0; i < P; i++){
		for(int j=0; j<p_params[i].c;j++){
			p_tmpResult->pli_list[c] = p_params[i].pli_list[j];
			c++;
		}
	}
	
	return p_tmpResult;
}
