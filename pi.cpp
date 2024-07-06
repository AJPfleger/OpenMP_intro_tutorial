/*

This program will numerically compute the integral of

                  4/(1+x*x) 
				  
from 0 to 1.  The value of this integral is pi -- which 
is great since it gives us an easy way to check the answer.

The is the original sequential program.  It uses the timer
from the OpenMP runtime library

History: Written by Tim Mattson, 11/99.

cpp-port: AJPfleger

*/
#include <stdio.h>
#include <omp.h>

int main () {
  const long num_steps = 100000000;
  constexpr double step = 1.0 / static_cast<double>(num_steps);

  double x = 0.0;
  double pi = 0.0;
  double sum = 0.0;

  const double start_time = omp_get_wtime();

  for (long i = 1; i <= num_steps; i++) {
    x = (i - 0.5) * step;
    sum = sum + 4.0 / (1.0 + x * x);
  }

  pi = step * sum;

  const double run_time = omp_get_wtime() - start_time;

  printf("\n pi with %ld steps is %lf in %lf seconds\n ", num_steps, pi, run_time);
}
