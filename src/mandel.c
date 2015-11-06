#include <stdio.h>
#include <stdint.h>
#include <complex.h>

const double mandel_pseudo_infinity = 500000;

int mandel_run_iterations(
    double r_re, double r_im,
    double z_re, double z_im,
    unsigned int iter_count) {

  // A configurable parameter of mandelbrot set.
  // In fact, it's an {x,y} coordinate in the resulting picture.
  double complex r = r_re + r_im * I;

  // Value of initial mandelbrot set iteration also known as z{0}.
  double complex z = z_re + z_im * I;
  double z_abs = cabs(z);

  // Intermediate storage of z to compare with previous value.
  double complex z_next;
  double z_next_abs;

  // Counter, which contains a number of z iterations
  // that were bigger than z previous.
  unsigned int result_count = 0;

  // Other variables
  unsigned int i;

  for (i = 0; i < iter_count; i++) {
    // Do iteration on this formula
    // where z is z{n}, and z_next is z{n+1}.
    z_next     = r * z * (1 - z);
    z_next_abs = cabs(z_next);

    // Don't run any more iterations if value tends to infinity.
    // NOTE: This is a super cheap optimization!
    if (z_next_abs > mandel_pseudo_infinity) {
      // Return current counter + all iterations left.
      return result_count + iter_count - i;
    }

    // Check equation and increase counter.
    if (z_next_abs >= z_abs) {
      result_count++;
    }

    // Set z to be of current iteration.
    z     = z_next;
    z_abs = z_next_abs;
  }

  return result_count;

}

void mandel_render_block(
    unsigned int *buf,
    int size_x,
    int size_y,
    int offset_x,
    int offset_y,
    double scale_x,
    double scale_y,
    double r_re, double r_im,
    double z_re, double z_im,
    unsigned int iter_count) {

  int x, y;

  // Run iterations and fill the buffer
  for (y = 0; y < size_y; y++) {
    for (x = 0; x < size_x; x++) {
      buf[size_x * y + x] = mandel_run_iterations(
        scale_x * (x + offset_x) + r_re,
        scale_y * (y + offset_y) + r_im,
        z_re, z_im, iter_count
      );
    }
  }

}
