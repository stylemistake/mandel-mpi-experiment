# Makefile

export CC_FLAGS := -std=c99 -lm -O3 -s -ffast-math -march=native -mtune=native

.DEFAULT_GOAL := default
.PHONY: default clean

default: bin/mandel
mpi: bin/mandel-mpi

bin/mandel: src/main.c src/mandel.c
	@mkdir -p bin
	gcc $(CC_FLAGS) src/main.c -o bin/mandel

bin/mandel-mpi: src/main_mpi.c src/mandel.c
	@mkdir -p bin
	mpicc $(CC_FLAGS) src/main_mpi.c -o bin/mandel-mpi

clean:
	@rm -rf bin *.bin *.out
