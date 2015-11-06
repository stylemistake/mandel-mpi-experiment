#include <stdio.h>
#include <stdint.h>
#include <complex.h>

const double mandel_pseudo_infinity = 1000000;

int mandel_run_iterations(
    // A configurable parameter of mandelbrot set.
    // In fact, it's an {x,y} coordinate in the resulting picture.
    double complex r,

    // Value of initial mandelbrot set iteration also known as z{0}.
    double complex z,

    // Iteration count
    unsigned int n) {

  // Pre-initialize absolute value of z{n}
  double z_abs = cabs(z);

  // Intermediate storage of z{n+1} to compare with z{n}.
  double complex z_next;
  double z_next_abs;

  // Counter, which contains a number of z{n} iterations
  // that were bigger than z{n+1}.
  unsigned int result_count = 0;

  // Other variables
  unsigned int i;

  for (i = 0; i < n; i++) {
    // Do iteration on this formula
    // where z is z{n}, and z_next is z{n+1}.
    z_next     = r * z * (1 - z);
    z_next_abs = cabs(z_next);

    // Don't run any more iterations if value doesn't change with iterations.
    // NOTE: This is a super cheap optimization, but image loses some of its
    // detail, especially in inner parts.
    if (z_next_abs == z_abs) {
      return result_count + n - i;
    }

    // Don't run any more iterations if value tends to infinity.
    // NOTE: This is more of a tradeoff rather than optimization.
    if (z_next_abs > mandel_pseudo_infinity) {
      // Return current counter + all iterations left.
      return result_count + n - i;
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

void mandel_render_block_uint32_t(
    uint32_t *buf,
    int size_x,
    int size_y,
    int offset_x,
    int offset_y,
    double scale_x,
    double scale_y,
    double r_re, double r_im,
    double z_re, double z_im,
    unsigned int n) {

  int x, y;
  double complex r;
  double complex z = z_re + z_im * I;

  // Run iterations and fill the buffer
  for (y = 0; y < size_y; y++) {
    for (x = 0; x < size_x; x++) {
      // Update r to refrect chosen viewport offsets
      r = (scale_x * (x + offset_x) + r_re)
        + (scale_y * (y + offset_y) + r_im) * I;
      buf[size_x * y + x] = mandel_run_iterations(r, z, n);
    }
  }

}

// TODO: Fix code duplication by migrating to f....ng C++
void mandel_render_block_uint16_t(
    uint16_t *buf,
    int size_x,
    int size_y,
    int offset_x,
    int offset_y,
    double scale_x,
    double scale_y,
    double r_re, double r_im,
    double z_re, double z_im,
    unsigned int n) {

  int x, y;
  double complex r;
  double complex z = z_re + z_im * I;

  // Run iterations and fill the buffer
  for (y = 0; y < size_y; y++) {
    for (x = 0; x < size_x; x++) {
      // Update r to refrect chosen viewport offsets
      r = (scale_x * (x + offset_x) + r_re)
        + (scale_y * (y + offset_y) + r_im) * I;
      buf[size_x * y + x] = mandel_run_iterations(r, z, n);
    }
  }

}

void mandel_render_block_uint8_t(
    uint16_t *buf,
    int size_x,
    int size_y,
    int offset_x,
    int offset_y,
    double scale_x,
    double scale_y,
    double r_re, double r_im,
    double z_re, double z_im,
    unsigned int n) {

  int x, y;
  double complex r;
  double complex z = z_re + z_im * I;

  // Run iterations and fill the buffer
  for (y = 0; y < size_y; y++) {
    for (x = 0; x < size_x; x++) {
      // Update r to refrect chosen viewport offsets
      r = (scale_x * (x + offset_x) + r_re)
        + (scale_y * (y + offset_y) + r_im) * I;
      buf[size_x * y + x] = mandel_run_iterations(r, z, n);
    }
  }

}
