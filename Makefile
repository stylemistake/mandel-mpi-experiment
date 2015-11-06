# Makefile

.DEFAULT_GOAL := default
.PHONY: default clean

default: bin/mandel

bin/mandel: src/main.c src/mandel.c
	gcc -std=c99 -lm -O3 -march=native -mtune=native src/main.c -o bin/mandel

clean:
	rm -f bin/mandel
