#!/bin/bash
cd `dirname ${0}`
make || exit ${?}
bin/mandel "${@}" | bin/image_view.py
