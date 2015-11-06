#include <stdio.h>
#include <mpi.h>

#include "mandel.c"

#define TAG_CHUNK_ID 2001
#define TAG_CHUNK_DATA 2002

#define IMAGE_WIDTH 1600
#define IMAGE_HEIGHT 800

#define MANDEL_SCALE 0.005
#define MANDEL_OFFSET_X 2
#define MANDEL_OFFSET_Y 0
#define MANDEL_ITERATIONS 1024

int main(int argc, char **argv) {

  int ierr;
  int proc_rank, proc_total;
  int proc_request;
  MPI_Status status;

  const int im_width = IMAGE_WIDTH;
  const int im_height = IMAGE_HEIGHT;

  int im_chunk_height;
  int im_chunk_offset;
  int i;

  // Image metadata
  uint32_t im_header[] = { im_width, im_height, sizeof(int) * 8 };
  unsigned int im_buf[im_width * im_height];

  // MPI spawns a process cluster from right here
  ierr = MPI_Init(&argc, &argv);

  ierr = MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
  ierr = MPI_Comm_size(MPI_COMM_WORLD, &proc_total);

  // Calculate chunk offsets for this process
  im_chunk_offset = im_height * proc_rank / proc_total;
  im_chunk_height = im_height * (proc_rank + 1) / proc_total - im_chunk_offset;

  fprintf(stderr, "proc-%d: rendering chunk %d..%d\n",
    proc_rank, im_chunk_offset, im_chunk_height + im_chunk_offset
  );

  // Render the chunk into the target buffer
  mandel_render_block(
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
    fwrite(&im_header, sizeof(uint32_t), 3, stdout);
    fflush(stdout);

    fprintf(stderr, "proc-%d: writing chunk %d..%d\n",
      proc_rank, im_chunk_offset, im_chunk_height + im_chunk_offset
    );

    // Write chunk of the master process
    fwrite(&im_buf, sizeof(int), im_width * im_chunk_height, stdout);
    fflush(stdout);

    // Receive chunks from other processes
    for (i = 1; i < proc_total; i++) {
      // Calculate chunk offsets for this process
      im_chunk_offset = im_height * i / proc_total;
      im_chunk_height = im_height * (i + 1) / proc_total - im_chunk_offset;

      fprintf(stderr, "proc-%d: receiving chunk %d..%d\n",
        proc_rank, im_chunk_offset, im_chunk_height + im_chunk_offset
      );

      // Receive the chunk
      ierr = MPI_Recv(
        &im_buf, im_width * im_chunk_height, MPI_INT,
        i, TAG_CHUNK_DATA, MPI_COMM_WORLD, &status
      );

      fprintf(stderr, "proc-%d: writing chunk %d..%d\n",
        proc_rank, im_chunk_offset, im_chunk_height + im_chunk_offset
      );

      // Write chunk
      fwrite(&im_buf, sizeof(int), im_width * im_chunk_height, stdout);
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
      &im_buf, im_width * im_chunk_height, MPI_INT,
      0, TAG_CHUNK_DATA, MPI_COMM_WORLD
    );

  }

  ierr = MPI_Finalize();

  return 0;

}
