#!/bin/zsh

# Be sure to make tests with D=1

source tools/test-helpers.zsh

# set -x

PROGRAM=$1
OUTPUT=${PROGRAM%.x}.out
shift
LAUNCH=${*}

eval ${LAUNCH} -n 2 ${PROGRAM} > ${OUTPUT} 2>&1
CODE=$?
[[ ${CODE} == 0 ]] || crash "exit code was: ${CODE}"

# Should be 5 calls to handle_ping()...
N=$( grep -c "handle_ping" ${OUTPUT} )
(( N == 5 )) || crash "N != 6"

grep "handle_quit" ${OUTPUT} > /dev/null || crash "No handle_quit!"

# Should be 2 "Normal exit."s
N=$( grep -c "Normal exit." ${OUTPUT} )
(( N == 2 )) || crash "N != 2"

exit 0
