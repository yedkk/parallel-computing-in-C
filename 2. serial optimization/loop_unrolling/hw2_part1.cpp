#include <math.h>
//author Kangdong Yuan
// complex algorithm for evaluation
void myfunc(double *s, double *mat, int *v, int length)
{
	// this assumes that the two dimensional vector is square 
	//create all varible for computation in future
	double val, a,b,t;
	
	//change the order or loop to enable me to reuse some varibales
	for (int i = 0; i < length; i ++)	
	{
		//calculate the val in outer loop, it will save computation time
		//round function is useless, when it deal with a integer
		//delete round function to save time
		val = fmod(v[i], 256);
		//calculate the sin and cos of val before and assign to variable
		a = sin(val);
		b = cos(val);
		//calculate the store value to aviod waste time
		t = (a*a-b*b);
		for (int j = 0; j < length; j++)
		{
			//calculate based on store value
			mat[i + j * length] = s[i + j * length]*t;
		}
	}
}

