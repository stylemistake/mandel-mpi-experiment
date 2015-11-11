#!/bin/bash
cd `dirname ${0}`/..
make mpi || exit ${?}
mpirun -np 0 bin/mandel-mpi "${@}" | python3 utils/view.py
