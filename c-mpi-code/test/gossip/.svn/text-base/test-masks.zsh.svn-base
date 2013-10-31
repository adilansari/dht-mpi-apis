#!/bin/zsh

# Be sure to make tests with D=1

source tools/test-helpers.zsh

# set -x

PROGRAM=$1
OUTPUT=${PROGRAM%.x}.out

${LAUNCH} ${PROGRAM} > ${OUTPUT} 2>&1
CODE=$?
[[ ${CODE} == 0 ]] || crash "exit code was: ${CODE}"

# Should be 4 "Debug mask"s
N=$( grep -c "Debug mask" ${OUTPUT} )
(( N == 4 )) || crash "N != 4"

# Should be 2 line outputs
N=$( grep -c "test-masks.c main.*Debug mask" ${OUTPUT} )
(( N == 2 )) || crash "N != 2"

exit 0
