#include <stdio.h>
#include <stdint.h>

#include "mandel.c"

#define IMAGE_WIDTH 800
#define IMAGE_HEIGHT 480

#define MANDEL_SCALE 0.005
#define MANDEL_OFFSET_X 2
#define MANDEL_OFFSET_Y 0
#define MANDEL_ITERATIONS 1024

int main(int argc, char **argv) {

  const int im_width = IMAGE_WIDTH;
  const int im_height = IMAGE_HEIGHT;

  // Image metadata
  uint32_t im_header[] = { im_width, im_height, 16 };
  uint16_t im_buf[im_width];

  unsigned int i;

  // Write headers
  fwrite(&im_header, sizeof(uint32_t), 3, stdout);
  fflush(stdout);

  for (i = 0; i < im_width; i++) {
    // Fill the buffers
    mandel_render_block_uint16_t(
      im_buf, im_width, 1,
      -im_width / 2, -im_height / 2 + i,
      MANDEL_SCALE, MANDEL_SCALE,
      MANDEL_OFFSET_X, MANDEL_OFFSET_Y,
      0.4, 0.0, MANDEL_ITERATIONS
    );

    // Write line
    fwrite(&im_buf, sizeof(uint16_t), im_width, stdout);
    fflush(stdout);
  }

  return 0;

}
