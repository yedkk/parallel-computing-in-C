#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>

void get_walltime(double* wcTime) {
   struct timeval tp;
   gettimeofday(&tp, NULL);
   *wcTime = (double)(tp.tv_sec + tp.tv_usec/1000000.0);
 }
void dummy(double *A, double *B, double *C, double *D) {}


//author Kangdong Yuan
int main(int argc,char *argv[]){
 int N =1000;
  if(argc!=1)
     N=atoi(argv[1]);
  int R=1000000;
  double *A,*B,*C,*D;
  A=(double*)calloc(N,sizeof(double));
  B=(double*)calloc(N,sizeof(double));
  C=(double*)calloc(N,sizeof(double));
  D=(double*)calloc(N,sizeof(double));
  double S,E,MFLOPS;
  int i;
  for(i=0;i<N;i++){  //initialize arrays
     A[i]=0e0;B[i]=1e0;
     C[i]=2e0;D[i]=3e0;
  }
 get_walltime(&S);  // get time stamp
int j;
 for(j=0;j<R;j++){
    for(i=0;i<N;i++){
      A[i]=B[i]+C[i]*D[i];      // 3 loads, 1 store
    }
    if (A[1] < 0) {
        dummy(A, B, C, D);  // prevent loop interchange
    }
  }
  get_walltime(&E);  // get time stamp
  MFLOPS=R*N*2e0/((E-S)*1e6);  //compute MFlop/sec rate
  printf("The MFLOPS is %f",MFLOPS);  
  return 0;
}
