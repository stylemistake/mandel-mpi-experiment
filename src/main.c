#include <stdio.h>
#include <stdint.h>

#include "mandel.c"

int main(int argc, char **argv) {

  const int im_width = 800;
  const int im_height = 480;

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
      0.005, 0.005,
      0, 0,
      0.4, 0.0, 512
    );

    // Write line
    fwrite(&buf, sizeof(int), im_width, stdout);
    fflush(stdout);
  }

  return 0;

}
