#include <mpi.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <string.h>
#define TRUE 1
#define FALSE 0
//author Kangdong Yuan
void performSmallMM(float *A, float *B, float *C, int N)
{
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
		{
				for (int k = 0; k < N; k++)
				{
					C[i * N + j] += A[i * N + k] * B[k * N + j];
				}
		}
}

void printNodes(float *A, float *B, float *C, int N)
{	std::cout<<"A:"<<std::endl;
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			std::cout<<A[i*N+j];
		}
		std::cout<<std::endl;
	}

	std::cout<<"B:"<<std::endl;
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			std::cout<<B[i*N+j];
		}
		std::cout<<std::endl;
	}
	std::cout<<"C:"<<std::endl;
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			std::cout<<C[i*N+j];
		}
		std::cout<<std::endl;
	}
}

void shiftrows(float *data, int N, int shiftnum, int mylocx, int mylocy, MPI_Comm vu)
{
	int right, left;
	float *tmpdata = new float[N*N];
	MPI_Cart_shift(vu, 0, shiftnum, &left, &right);
	MPI_Sendrecv(data, N*N, MPI_FLOAT, left, 0, tmpdata, N*N, MPI_FLOAT,
	right, 0, vu, MPI_STATUS_IGNORE);
	
	memcpy(data, tmpdata, N*N*sizeof(float));
	delete[] tmpdata;
}

void shiftcols(float *data, int N, int shiftnum, int mylocx, int mylocy, MPI_Comm vu)
{
	int up, down;
	float *tmpdata = new float[N*N]; 
	MPI_Cart_shift(vu, 1, shiftnum, &up, &down);
	MPI_Sendrecv(data, N*N, MPI_FLOAT, up, 0, tmpdata, N*N, MPI_FLOAT, 
	down, 0,  vu, MPI_STATUS_IGNORE);
	
	memcpy(data, tmpdata, N*N*sizeof(float));
	delete[] tmpdata;
}

void filltmp(float *A, float *tmpA, int x, int y, int N, int N_prime, int p_prime)
{
	for (int ii =0; ii<N_prime; ii++)
	{
		for (int jj =0; jj<N_prime; jj++)
		{	
			tmpA[ii*N_prime+jj]=A[((y* N_prime)+ii)*N+x*N_prime+ii];
		} 
	}
}

void printcartmap(int *dim, MPI_Comm vu)
{
	int coord[2];
	int cartrank;
	for (int y=0; y<dim[1]; y++)
	{
		for (int x=0; y<dim[0]; x++)
		{
			coord[0]=x;
			coord[1]=y;
			MPI_Cart_rank(vu,coord,&cartrank);
			std::cout<<cartrank<<",";
			
		}
		std::cout<<std::endl;
	}
	
	
}



void recvblock(float *block, int N, MPI_Comm vu)
{ 
	MPI_Recv(block, N*N, MPI_FLOAT, MPI_ANY_SOURCE, 0, vu, MPI_STATUS_IGNORE);
}



int main(int argc, char *argv[]) {
	
	//N is the dismension
	int N=10;
	
	int myloc[2], dimx, dimy;
	int N_prime;
	float *A,*B, *C, *tmpCalcA, *tmpCalcB, *tmpA, *tmpB;
	
	 
	//set the mpi
	int rank, oldrank;
	MPI_Comm vu;
	
	int dim[2],period[2],reorder;
	int up,down,right,left;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	
	int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	
	int p_prime = floor(sqrt(world_size));
	dimx=p_prime;
	dimy=p_prime;
	dim[0]=dimx; dim[1]=dimy;
	period[0]=1; period[1]=1;
	reorder=0;
	
	MPI_Cart_create(MPI_COMM_WORLD,2,dim,period,reorder,&vu);
	
	
	oldrank =rank;
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	
	//location
	MPI_Cart_coords(vu, rank, 2, myloc);
	
	if (rank==0)
	{
		//printcartmap(dim,vu);
	}
	
	if (N%p_prime!=0)
	{ 
		printf("This is bad\n");
		MPI_Finalize();
		return -1;
	}
	
	//local space
	N_prime = N/p_prime;
	
	std::cout<<"N="<<N<<", N_prime= "<<N_prime<<std::endl;
	std::cout<<"P="<<world_size<<", p_prime= "<<p_prime<<std::endl;
	
	float *bigA, *bigB, *bigC;
	
	A = new float[N_prime*N_prime];
	B = new float[N_prime*N_prime];
	C = new float[N_prime*N_prime];
	tmpA = new float[N_prime*N_prime];
	tmpB = new float[N_prime*N_prime];
	
	if (rank==0)
	{
		bigA = new float[N*N];
		bigB = new float[N*N];
		for (int ii =0; ii<N; ii++)
		{
			for (int jj =0; jj<N; jj++)
			{
				bigA[ii*N+jj]=ii;
				bigB[ii*N+jj]=0;
			}
			bigB[ii*N+ii]=2;
		}
		int coord[2], id;
		for (int ii =0; ii<p_prime; ii++)
		{
			for (int jj =0; jj<p_prime ; jj++)
			{
				coord[0]=jj;
				coord[1]=ii;
				MPI_Cart_rank(vu,coord,&id);
				filltmp(bigA, tmpA, coord[0], coord[1], N, N_prime, p_prime);
				MPI_Send(tmpA, N_prime*N_prime , MPI_FLOAT, id, 0, vu);
				filltmp(bigB, tmpB, coord[0], coord[1], N, N_prime, p_prime);
				MPI_Send(tmpB, N_prime*N_prime , MPI_FLOAT, id, 0, vu);
			}
		}
		filltmp(bigA, A, coord[0], coord[1], N, N_prime, p_prime);
		filltmp(bigB, B, coord[0], coord[1], N, N_prime, p_prime);
	}
	else
	{
		recvblock(A, N_prime, vu);
		recvblock(B, N_prime, vu);
	}
	
	for (int ii=0; ii<(N_prime*N_prime); ii++)
	{
		//A[ii] =rank;
		//B[ii] =0;
		C[ii] =0;
	}
	

	
	//////////////////////////////////////
	//init
	/*
	for (int i =1; i <p_prime; i++)
	{
		shiftrows(A, N_prime, i, myloc[0], myloc[1],  vu);
		shiftcols(B, N_prime, i, myloc[0], myloc[1],  vu);
	}*/
	
	if (myloc[1]>0)
	{
		shiftrows(A, N_prime, myloc[1], myloc[0], myloc[1], vu);
	}
	
	MPI_Barrier(vu);
	
	if (myloc[0]>0)
	{
		shiftcols(B, N_prime, myloc[0], myloc[0], myloc[1], vu);
	}

	MPI_Barrier(vu);
	
	
	for (int i =0; i < p_prime; i++)
	{
		//calculate
		performSmallMM(A, B, C, N_prime);
		
		if (i<p_prime-1)
		{	
			//phase1
			shiftrows(A, N_prime, 1, myloc[0], myloc[1], vu);
			
			//MPI_Barrier(vu)
		
			//phase2
			shiftcols(B, N_prime, 1, myloc[0], myloc[1], vu);
		}
	}

	if (rank==0)
	{	
		bigC=new float[N*N];
		std::cout<<"Full answer: "<<std::endl;
		performSmallMM(bigA, bigB, bigC, N);
		printNodes(bigA, bigB, bigC, N);
		std::cout<<"Rank: "<<rank<<"Coords: "<<myloc[0]<<", "<<myloc[1]<<std::endl;		
		printNodes(A,B,C ,N_prime);
		MPI_Send(&rank, 1 , MPI_INT, rank+1, 0, vu);
	}
	else
	{
		int tmpI;		
		MPI_Recv(&tmpI, 1, MPI_INT, MPI_ANY_SOURCE, 0, vu, MPI_STATUS_IGNORE);
		std::cout<<"Rank: "<<rank<<"Coords: "<<myloc[0]<<", "<<myloc[1]<<std::endl;
		printNodes(A,B,C ,N_prime);
		if (rank<world_size-1)
		{
			MPI_Send(&rank, 1 , MPI_INT, rank+1, 0, vu);
		}
	}
	
	MPI_Finalize();
	return 0;
}
