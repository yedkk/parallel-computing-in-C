/**
 * CMPCS-450 Assignment 3
 * sd_thread.h
 * Warning: Do not change anything in this file
 * 
 * @author Niranjan Thirusangu, William Seisler
 * @version 1.2 02/18/2020
*/
#ifndef SD_THREAD
#define SD_THREAD

// a structure containing the statistics of the dataset (self defined)
struct STDDEV_RESULT
{
	double mean;
	double min;						
	double max;
	double stddev;
};
	
struct THRESH_RESULT
{
	long int *pli_list;				// an array containing indexes of 
									//     values over threshold
	long int li_threshCount;		// size of pli_list
};
/**
    Returns the mean, min, max and Standard deviation of a given array.

	@param A Pointer to the first element of the A array.
    @param N Number of elements in the A array.
    @param P Number of threads the function should use.
    @return mean, min, max and sd using result structure.
*/
STDDEV_RESULT* calcSdThread(double *A, long N, int P);

/**
    Returns a list of indices of values over a threshold in a given array.

	@param A Pointer to the first element of the A array.
    @param N Number of elements in the A array.
    @param P Number of threads the function should use.
    @return threshold list and count in threshold structure.
*/
THRESH_RESULT *findThreshValuesThread(double *A, long N, double T, int P);
#endif
