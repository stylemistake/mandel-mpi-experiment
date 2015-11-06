#!/bin/bash
cd `dirname ${0}`/..
make || exit ${?}
bin/mandel "${@}" | python3 utils/view.py
