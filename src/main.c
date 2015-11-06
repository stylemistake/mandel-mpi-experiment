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
  uint32_t header[] = { im_width, im_height, sizeof(int) * 8 };
  unsigned int buf[im_width];

  unsigned int i;

  // Write headers
  fwrite(&header, sizeof(uint32_t), 3, stdout);
  fflush(stdout);

  for (i = 0; i < im_width; i++) {
    // Fill the buffers
    mandel_render_block(
      buf, im_width, 1,
      -im_width / 2, -im_height / 2 + i,
      MANDEL_SCALE, MANDEL_SCALE,
      MANDEL_OFFSET_X, MANDEL_OFFSET_Y,
      0.4, 0.0, MANDEL_ITERATIONS
    );

    // Write line
    fwrite(&buf, sizeof(int), im_width, stdout);
    fflush(stdout);
  }

  return 0;

}
