/*
**  PROGRAM: Mandelbrot area
**
**  PURPOSE: Program to compute the area of a  Mandelbrot set.
**           Correct answer should be around 1.510659.
**           WARNING: this program may contain errors
**
**  USAGE:   Program runs without input ... just run the executable
**            
**  HISTORY: Written:  (Mark Bull, August 2011).
**           Changed "comples" to "d_comples" to avoid collsion with 
**           math.h complex type (Tim Mattson, September 2011)
** cpp adaption: AJPfleger
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

struct d_complex{
    double r;
    double i;
};

// Does the iteration z=z*z+c, until |z| > 2 when point is known to be outside set
// If loop count reaches MAXITER, point is considered to be inside the set
void testpoint(d_complex c, int& numoutside, const int max_iter = 1000){
  struct d_complex z = c;

  for (int iter = 0; iter < max_iter; iter++) {
    double temp = (z.r * z.r) - (z.i * z.i) + c.r;
    z.i = z.r * z.i * 2 + c.i;
    z.r = temp;

    if ((z.r * z.r + z.i * z.i) > 4.) {
      #pragma omp atomic
      numoutside++;
      return;
    }
  }

  return;
}

int main() {
  const double eps = 1.e-5;
  const int n_points = 1000;
  const int max_iter = 1000;
  d_complex c;
  int numoutside = 0;

  //   Loop over grid of points in the complex plane which contains the Mandelbrot set,
  //   testing each point to see whether it is inside or outside the set.
  #pragma omp parallel for default(shared) private(c)
  for (int i = 0; i < n_points; i++) {
    for (int j = 0; j < n_points; j++) {
      c.r = -2. + 2.5 * static_cast<double>(i) / static_cast<double>(n_points) + eps;
      c.i = 1.125 * static_cast<double>(j) / static_cast<double>(n_points) + eps;
      testpoint(c, numoutside, max_iter);
    }
  }

  // Calculate area of set and error estimate and output the results
  const double area = 2. * 2.5 * 1.125 *
      static_cast<double>(n_points * n_points - numoutside) /
      static_cast<double>(n_points * n_points);
  const double error = area / static_cast<double>(n_points);

  printf("Area of Mandelbrot set = %12.8f +/- %12.8f\n", area, error);
  printf("Correct answer should be around 1.510659\n");

}



