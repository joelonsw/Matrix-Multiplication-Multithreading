#define SIZE 4096
#define SIZE_THREAD 64

#include <iostream>
#include <string>
#include <cstdio>
#include <stdlib.h>
#include <x86intrin.h>
using namespace std;

long long wall_clock_time();
void zero_matrix(float *m);
void matrix_multiply(float *m1, float *m2, float *result);
void print_matrix(float *m);
void random_matrix(float *m);

struct MultipleArg
{
  int a;
  int b;
  int c;
  float *matA;
  float *matB;
  float *matC;
};

void zero_matrix(float *m) {
	for (int i = 0; i < SIZE; i++) 
		for (int j = 0; j < SIZE; j++) 
			m[i * SIZE + j] = 0;
}

void* multi(void* arg) 
{ 
  MultipleArg __attribute__((aligned(16))) *arg_thread = (MultipleArg *)arg;

  float temp;            
  for (int k = 0; k < SIZE/4; k+=64)  {
      for (int i = 0; i < SIZE/4; i+=64){
          for (int j = 0; j < SIZE/4; j+=64){
                for(int q=0; q<64; q++){
                  for(int p=0; p<64; p++){
                    temp = arg_thread->matA[arg_thread->a+(i*SIZE+k)+(p*SIZE+q)];
                    for(int r=0; r<64; r++){
				                arg_thread->matC[arg_thread->c+(i * SIZE + j)+(p*SIZE+r)] += temp * arg_thread->matB[arg_thread->b+(k * SIZE + j)+(q*SIZE+r)];
                    
                    }
                  }
              }
          }    
      }        
  }
  free(arg_thread);   
} 


void matrix_multiply_optimized(float *m1, float *m2, float *result) {
	long long start, end;
	start = wall_clock_time();
    pthread_t threads[SIZE_THREAD];
    MultipleArg *multiple_arg;
    for(int j=0; j<4 ; j++){
        for(int i=0; i<SIZE_THREAD/4; i++){         
            multiple_arg = (MultipleArg *)malloc(sizeof(MultipleArg));
            multiple_arg->a = (((i/4)*(SIZE/4))*(SIZE)) + ((j%4)*(SIZE/4));
            multiple_arg->b = (((j%4)*(SIZE/4))*(SIZE)) + ((i%4)*(SIZE/4));
            multiple_arg->c = (((i/4)*(SIZE/4))*(SIZE)) + ((i%4)*(SIZE/4));
            multiple_arg->matA = m1;
            multiple_arg->matB = m2;
            multiple_arg->matC = result;
            pthread_create(&threads[(j*(SIZE_THREAD/4))+i], NULL, multi, (void *) multiple_arg);            
            
        }
        for(int k=0; k<SIZE_THREAD/4; k++)
          pthread_join(threads[(j*(SIZE_THREAD/4))+k], NULL);
    }
	end = wall_clock_time();
	fprintf(stderr, "Optimized Matrix multiplication took %1.2f seconds\n", ((float)(end - start))/1000000000);
}


void print_matrix(float *m) {
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			cout << m[i * SIZE + j] << " ";
		} 
		cout << endl;
	}
}

void random_matrix(float *m) {
	for (int i = 0; i < SIZE; i++) 
		for (int j = 0; j < SIZE; j++) 
			m[i * SIZE + j] = rand() % 10;
}

int main() {
	// init matrixes
	float *m1 __attribute__((aligned(16))) = new float[SIZE * SIZE];
	float *m2 __attribute__((aligned(16))) = new float[SIZE * SIZE];
	float *result_optimized __attribute__((aligned(16))) = new float[SIZE * SIZE];
	random_matrix(m1);
	random_matrix(m2);
	zero_matrix(result_optimized);

	// do matrix multiply
	matrix_multiply_optimized(m1, m2, result_optimized);


  delete [] m1;
  delete [] m2;
  delete [] result_optimized;
  

	return 0;
}




/********************************************
 * Helpers
 *******************************************/
long long wall_clock_time() {
	#ifdef __linux__
		struct timespec tp;
		clock_gettime(CLOCK_REALTIME, &tp);
		return (long long)(tp.tv_nsec + (long long)tp.tv_sec * 1000000000ll);
	#else
		#warning "Your timer resoultion might be too low. Compile on Linux and link with librt"
		struct timeval tv;
		gettimeofday(&tv, NULL);
		return (long long)(tv.tv_usec * 1000 + (long long)tv.tv_sec * 1000000000ll);
	#endif
}
