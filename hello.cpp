#include <iostream>
#include <omp.h>

int main () {

  // omp_set_num_threads(4);
  // #pragma omp parallel
  // or
  // #pragma omp parallel num_threads(4)
  // or for maximum
  #pragma omp parallel
  {
    int threadId = omp_get_thread_num();
    #pragma omp critical
    {
      std::cout << "Hello World from ID " << threadId << std::endl;
    }
  }

  return 0;
}
