// complex algorithm for evaluation
//author: Kangdong Yuan 
# define BLOCKSIZE 20 //this is the block size that can be change
//define the helper function that help to do the the divided calculation
 void do_block (double *A, double *B, double *C, int N)

{
	//those for loops are try to do the matrix mult in blocksize
    for(int i = 0; i < BLOCKSIZE; i++)

    {
        for(int j = 0; j < BLOCKSIZE; j++)

        {
			//get the cij value
            double cij = C[i*N+j];

            for(int k = 0; k < BLOCKSIZE; k++ )
				
				//store the value before to avoid the too much access of the data
                cij +=A[i*N+k] * B[k*N + j];

                C[i*N+j] = cij;

        }

    }

}

 void matrix_mult (double *A, double *B, double *C, int N)

{
	//the for loop for blocksize computation
    for ( int sj = 0; sj < N; sj += BLOCKSIZE )

        for ( int si = 0; si < N; si += BLOCKSIZE )

            for ( int sk = 0; sk < N; sk += BLOCKSIZE )

				//call the helper function
                do_block(A+si*N+sk, B+sk*N+sj,  C+si*N+sj ,N);

}

