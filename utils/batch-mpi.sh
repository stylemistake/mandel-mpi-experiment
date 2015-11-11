#!/bin/bash
#SBATCH -D /scratch/lustre/home/alko9089/mandel
#SBATCH -C alpha
#SBATCH -p short
#SBATCH -n 64
path_local='/scratch/local/alko9089/mandel'

rm -f mandel.bin.gz
srun mkdir -p ${path_local}/bin
sbcast bin/mandel-mpi ${path_local}/bin/mandel-mpi
echo 'batch: starting...'
mpirun bin/mandel-mpi | gzip -9 > mandel.bin.gz
echo 'batch: finished!'
srun rm -rf ${path_local}
