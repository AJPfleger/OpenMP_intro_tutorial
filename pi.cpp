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

double calcPiVector(const long num_steps, const std::uint32_t num_threads) {
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

double calcPiCritical(const long num_steps, const std::uint32_t num_threads) {
  const double step = 1.0 / static_cast<double>(num_steps);
  double pi = 0.;

  #pragma omp parallel num_threads(num_threads)
  {
    const int id = omp_get_thread_num();
    double sub_sum = 0.;
    for (long i = num_steps - id; i > 0; i -= num_threads) {
      double x = (i - 0.5) * step;
      sub_sum += 4. / (1. + x * x);
    }

    #pragma omp critical
    pi += step * sub_sum;
  }

  return pi;
}

double calcPiAtomic(const long num_steps, const std::uint32_t num_threads) {
  const double step = 1.0 / static_cast<double>(num_steps);
  double pi = 0.;

  #pragma omp parallel num_threads(num_threads)
  {
    const int id = omp_get_thread_num();
    double sub_sum = 0.;
    for (long i = num_steps - id; i > 0; i -= num_threads) {
      double x = (i - 0.5) * step;
      sub_sum += 4. / (1. + x * x);
    }

    double sub_pi = step * sub_sum;

    #pragma omp atomic
    pi += sub_pi;
  }

  return pi;
}

double calcPiFor(const long num_steps, const std::uint32_t num_threads) {
  const double step = 1.0 / static_cast<double>(num_steps);
  double pi = 0.;

  #pragma omp parallel num_threads(num_threads)
  {
    double sub_sum = 0.;
    #pragma omp for
    for (long i = num_steps; i > 0; i--) {
      double x = (i - 0.5) * step;
      sub_sum += 4. / (1. + x * x);
    }

    double sub_pi = step * sub_sum;

    #pragma omp atomic
    pi += sub_pi;
  }

  return pi;
}

double calcPiReduction(const long num_steps, const std::uint32_t num_threads) {
  const double step = 1.0 / static_cast<double>(num_steps);
  double sum = 0.;

  #pragma omp parallel num_threads(num_threads)
  {
    #pragma omp for reduction(+:sum)
    for (long i = num_steps; i > 0; i--) {
      double x = (i - 0.5) * step;
      sum += 4. / (1. + x * x);
    }
  }

  double pi = step * sum;

  return pi;
}

int main () {
  const long num_steps = 100000000;
  const std::uint32_t min_threads = 1;
  const std::uint32_t max_threads = 8;

  std::cout << "Running " << num_steps << " steps:" << std::endl;

  std::cout << "\ncalcPiVector:" << std::endl;
  for (auto num_threads = min_threads; num_threads <= max_threads; num_threads++) {
    double start_time = omp_get_wtime();
    double pi = calcPiVector(num_steps, num_threads);
    double run_time = omp_get_wtime() - start_time;

    std::cout << num_threads << " threads: pi = " << pi << " in " << run_time
              << " seconds." << std::endl;
  }

  std::cout << "\ncalcPiCritical:" << std::endl;
  for (auto num_threads = min_threads; num_threads <= max_threads; num_threads++) {
    double start_time = omp_get_wtime();
    double pi = calcPiCritical(num_steps, num_threads);
    double run_time = omp_get_wtime() - start_time;

    std::cout << num_threads << " threads: pi = " << pi << " in " << run_time
              << " seconds." << std::endl;
  }

  std::cout << "\ncalcPiAtomic:" << std::endl;
  for (auto num_threads = min_threads; num_threads <= max_threads; num_threads++) {
    double start_time = omp_get_wtime();
    double pi = calcPiAtomic(num_steps, num_threads);
    double run_time = omp_get_wtime() - start_time;

    std::cout << num_threads << " threads: pi = " << pi << " in " << run_time
              << " seconds." << std::endl;
  }

  std::cout << "\ncalcPiFor:" << std::endl;
  for (auto num_threads = min_threads; num_threads <= max_threads; num_threads++) {
    double start_time = omp_get_wtime();
    double pi = calcPiFor(num_steps, num_threads);
    double run_time = omp_get_wtime() - start_time;

    std::cout << num_threads << " threads: pi = " << pi << " in " << run_time
              << " seconds." << std::endl;
  }

  std::cout << "\ncalcPiReduction:" << std::endl;
  for (auto num_threads = min_threads; num_threads <= max_threads; num_threads++) {
    double start_time = omp_get_wtime();
    double pi = calcPiReduction(num_steps, num_threads);
    double run_time = omp_get_wtime() - start_time;

    std::cout << num_threads << " threads: pi = " << pi << " in " << run_time
              << " seconds." << std::endl;
  }

  return 0;
}
