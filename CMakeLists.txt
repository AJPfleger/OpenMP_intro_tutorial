cmake_minimum_required(VERSION 3.10)
project(openmp_intro_tutorial)

set(CMAKE_CXX_STANDARD 20)

# Set the paths to the Homebrew-installed libomp
set(OpenMP_CXX_FLAGS "-Xpreprocessor -fopenmp -I/usr/local/Cellar/libomp/18.1.8/include")
set(OpenMP_CXX_LIB_NAMES "omp")
set(OpenMP_omp_LIBRARY "/usr/local/Cellar/libomp/18.1.8/lib/libomp.dylib")

# Add the flags and link the libraries
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
link_directories(/usr/local/Cellar)
include_directories(/usr/local/Cellar/libomp/18.1.8/include)

add_executable(hello hello.cpp)
target_link_libraries(hello ${OpenMP_omp_LIBRARY})

add_executable(pi pi.cpp)
target_link_libraries(pi ${OpenMP_omp_LIBRARY})

add_executable(mandel mandel.cpp)
target_link_libraries(mandel ${OpenMP_omp_LIBRARY})

add_executable(linked linked.cpp)
target_link_libraries(linked ${OpenMP_omp_LIBRARY})

add_executable(prod_cons prod_cons.cpp)
target_link_libraries(prod_cons ${OpenMP_omp_LIBRARY})

add_executable(pi_mc pi_mc.cpp)
target_link_libraries(pi_mc ${OpenMP_omp_LIBRARY})
