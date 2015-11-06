#!/bin/bash
cd `dirname ${0}`/..
make mpi || exit ${?}
bin/mandel-mpi "${@}" | python3 utils/view.py
