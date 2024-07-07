/*
**  PROGRAM: A simple serial producer/consumer program
**
**  One function generates (i.e. produces) an array of random values.  
**  A second functions consumes that array and sums it.
**
**  HISTORY: Written by Tim Mattson, April 2007.
**
** cpp adaption: AJPfleger
*/

#include <omp.h>
#include <stdlib.h>
#include <stdio.h>

/* function to fill an array with random numbers */
void fill_rand(const int length, double *a) {

  int RAND_MULT = 1366;
  int RAND_ADD = 150889;
  int RAND_MOD = 714025;
  int randy = 2531;

  for (int i = 0; i < length; i++) {
    randy = (RAND_MULT * randy + RAND_ADD) % RAND_MOD;
    *(a+i) = ((double) randy)/((double) RAND_MOD);
  }

  return;
}

/* function to sum the elements of an array */
double Sum_array(const int length, double *a) {
  double sum = 0.0;
  for (int i = 0; i < length; i++) {
    sum += *(a + i);
  }

  return sum;
}
  
int main() {
  const int n = 10000;

  double *A;
  A = (double *)malloc(n * sizeof(double));

  int flag = 0;

  double sum = -1.;

  double runtime = omp_get_wtime();

#pragma omp parallel sections
  {
#pragma omp section
    {
      // Producer: fill an array of data
      fill_rand(n, A);
#pragma omp flush
#pragma atomic write
      flag = 1;
#pragma omp flush (flag)
    }
#pragma omp section
    {
      int flg_tmp = 0;
      while (true) {
#pragma omp flush(flag)
#pragma omp atomic read
        flg_tmp= flag;
        if (flg_tmp == 1) break;
      }
#pragma omp flush

      // Consumer: sum the array
      sum = Sum_array(n, A);
    }

  }
  runtime = omp_get_wtime() - runtime;

  printf(" In %f seconds, The sum is %f \n", runtime, sum);
}
 
