#!/bin/bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
mkdir ${DIR}/build
cd ${DIR}/build || exit $?
cmake ../ || exit $?
make all || exit $?
ctest --output-on-failure || exit $?
