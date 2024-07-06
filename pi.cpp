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

#include <iostream>
#include <numeric>
#include <vector>

#include <omp.h>

double calcPi(const long num_steps, const std::uint32_t num_threads) {
  const double step = 1.0 / static_cast<double>(num_steps);
  std::vector<double> sum(num_threads, 0.);

  #pragma omp parallel num_threads(num_threads)
  {
    const int id = omp_get_thread_num();
    double sub_sum = 0.;
    for (long i = num_steps - id; i > 0; i -= num_threads) {
      double x = (i - 0.5) * step;
      sub_sum += 4. / (1. + x * x);
    }
    sum[id] = sub_sum;
  }

  double pi = step * std::accumulate(sum.begin(), sum.end(), 0.);

  return pi;
}

int main () {
  const long num_steps = 100000000;
  const std::uint32_t min_threads = 1;
  const std::uint32_t max_threads = 8;

  for (auto num_threads = min_threads; num_threads <= max_threads; num_threads++) {
    double start_time = omp_get_wtime();
    double pi = calcPi(num_steps, num_threads);
    double run_time = omp_get_wtime() - start_time;

    std::cout << num_threads << " threads: pi = " << pi << " in " << run_time
              << " seconds." << std::endl;
  }

  return 0;
}
