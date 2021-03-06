#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#include "mandel.c"

#define TAG_CHUNK_ID 2001
#define TAG_CHUNK_DATA 2002

#define IMAGE_WIDTH 800
#define IMAGE_HEIGHT 480

#define MANDEL_SCALE 0.000000002
#define MANDEL_OFFSET_X 2.015
#define MANDEL_OFFSET_Y -1.0
#define MANDEL_ITERATIONS 4096

int main(int argc, char **argv) {

  int ierr, proc_rank, proc_total, proc_request;
  MPI_Status status;

  // MPI spawns a process cluster from right here
  ierr = MPI_Init(&argc, &argv);
  ierr = MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
  ierr = MPI_Comm_size(MPI_COMM_WORLD, &proc_total);

  // Image dimensions
  const int im_width = IMAGE_WIDTH;
  const int im_height = IMAGE_HEIGHT;

  // Calculate chunk offsets for this process
  int im_chunk_offset = im_height * proc_rank / proc_total;
  int im_chunk_height = im_height * (proc_rank + 1) / proc_total - im_chunk_offset;

  // Image data
  uint32_t im_header[] = { im_width, im_height, sizeof(uint16_t) * 8 };
  uint16_t *im_buf = malloc(im_width * (im_chunk_height + 1) * sizeof(uint16_t));

  fprintf(stderr, "proc-%d: rendering chunk %d..%d\n",
    proc_rank, im_chunk_offset, im_chunk_height + im_chunk_offset
  );

  // Render the chunk into the target buffer
  mandel_render_block_uint16_t(
    im_buf, im_width, im_chunk_height,
    -im_width / 2, -im_height / 2 + im_chunk_offset,
    MANDEL_SCALE, MANDEL_SCALE,
    MANDEL_OFFSET_X, MANDEL_OFFSET_Y,
    0.4, 0, MANDEL_ITERATIONS
  );

  // Master process
  if (proc_rank == 0) {

    fprintf(stderr, "proc-%d: writing headers %d..%d\n",
      proc_rank, im_chunk_offset, im_chunk_height + im_chunk_offset
    );

    // Write headers
    fwrite(im_header, sizeof(uint32_t), 3, stdout);
    fflush(stdout);

    fprintf(stderr, "proc-%d: writing chunk %d..%d\n",
      proc_rank, im_chunk_offset, im_chunk_height + im_chunk_offset
    );

    // Write chunk of the master process
    fwrite(im_buf, sizeof(uint16_t), im_width * im_chunk_height, stdout);
    fflush(stdout);

    // Receive chunks from other processes
    for (int i = 1; i < proc_total; i++) {
      // Calculate chunk offsets for this process
      im_chunk_offset = im_height * i / proc_total;
      im_chunk_height = im_height * (i + 1) / proc_total - im_chunk_offset;

      fprintf(stderr, "proc-%d: receiving chunk %d..%d\n",
        proc_rank, im_chunk_offset, im_chunk_height + im_chunk_offset
      );

      // Receive the chunk
      ierr = MPI_Recv(
        im_buf, im_width * im_chunk_height, MPI_SHORT,
        i, TAG_CHUNK_DATA, MPI_COMM_WORLD, &status
      );

      fprintf(stderr, "proc-%d: writing chunk %d..%d\n",
        proc_rank, im_chunk_offset, im_chunk_height + im_chunk_offset
      );

      // Write chunk
      fwrite(im_buf, sizeof(uint16_t), im_width * im_chunk_height, stdout);
      fflush(stdout);
    }

  }

  // Slave process
  else {

    fprintf(stderr, "proc-%d: sending chunk %d..%d\n",
      proc_rank, im_chunk_offset, im_chunk_height + im_chunk_offset
    );

    // Send the chunk
    ierr = MPI_Send(
      im_buf, im_width * im_chunk_height, MPI_SHORT,
      0, TAG_CHUNK_DATA, MPI_COMM_WORLD
    );

  }

  ierr = MPI_Finalize();

  return 0;

}
