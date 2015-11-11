# Mandelbrot generator

This is my attempt of making use of MPI for parallel processing.

Dependends on:

* GNU C Compiler 4.x
* Python 3 (for `utils/view.py`)
* Python PIL (for `utils/view.py`)
* OpenMPI (optional)

Run `make` to compile.

To generate and view the image:

```
$ bin/mandel | utils/view.py
```

You can also run this script for convenience:

```
$ bash utils/run.sh
```

Using pipes is faster, but you can use an intermediate file if you want, for
example to transfer it between computers (with gzip compression):

```
$ bin/mandel | gzip -9 > out.raw.gz
$ zcat out.raw.gz | utils/view.py
```

You can also compile and run an MPI version:

```
$ make mpi
$ mpirun -np 2 bin/mandel | gzip -9 > out.raw.gz

proc-0: rendering chunk 0..400
proc-1: rendering chunk 400..800
proc-1: sending chunk 400..800
proc-0: writing headers 0..400
proc-0: writing chunk 0..400
proc-0: receiving chunk 400..800
proc-0: writing chunk 400..800
```

Reference *slurm* batch file is located at `utils/batch-mpi.sh`.


## Contacts

Style Mistake <[stylemistake@gmail.com]>

[stylemistake.com]: http://stylemistake.com
[stylemistake@gmail.com]: mailto:stylemistake@gmail.com
