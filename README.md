# Mandelbrot generator

This is my attempt of making use of MPI for parallel processing.

Dependends on:

* OpenMPI (optional)
* Python 3 (for `image_decode.py`)
* Python PIL (for `image_decode.py`)

Run `make` to compile C source.

To generate and view the image:

```
$ bin/mandel | bin/image_view.py
```

Using pipes is faster, but you can use an intermediate file if you want:

```
$ bin/mandel > out.bin
$ cat out.bin | bin/image_view.py
```

You can also compile and run MPI version manually:

```
$ mpicc -std=c99 -lm -O3 -march=native -mtune=native src/main_mpi.c -o bin/mandel
$ mpirun -np 2 bin/mandel > out.bin

proc-0: rendering chunk 0..400
proc-1: rendering chunk 400..800
proc-1: sending chunk 400..800
proc-0: writing headers 0..400
proc-0: writing chunk 0..400
proc-0: receiving chunk 400..800
proc-0: writing chunk 400..800
```


## Contacts

Style Mistake <[stylemistake@gmail.com]>

[stylemistake.com]: http://stylemistake.com
[stylemistake@gmail.com]: mailto:stylemistake@gmail.com
