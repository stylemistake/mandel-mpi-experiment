#!/bin/bash
cd `dirname ${0}`
bin/mandel "${@}" | bin/image_view.py
