#include <iostream>
#include <omp.h>

int main () {

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
