#!/bin/zsh

# Be sure to make tests with D=1

source tools/test-helpers.zsh

# set -x

PROGRAM=$1
OUTPUT=${PROGRAM%.x}.out

crash()
{
  print $1
  mv ${OUTPUT} ${OUT}.failed
  exit 1
}

${LAUNCH} ${PROGRAM} > ${OUTPUT} 2>&1
CODE=$?
[[ ${CODE} == 0 ]] || crash "exit code was: ${CODE}"

CMPI_DEBUG_MASK=example ${LAUNCH} ${PROGRAM} >> ${OUTPUT} 2>&1
CODE=$?
[[ ${CODE} == 0 ]] || crash "exit code was: ${CODE}"

CMPI_DEBUG_MASK=all ${LAUNCH} ${PROGRAM} >> ${OUTPUT} 2>&1
CODE=$?
[[ ${CODE} == 0 ]] || crash "exit code was: ${CODE}"

# Should be 3 "test program"s
N=$( grep -c "test program" ${OUTPUT} )
(( N == 3 )) || crash "N != 3"

# Should be 2 "Example debugging message."s
N=$( grep -c "EXAMPLE MESSAGE" ${OUTPUT} )
(( N == 2 )) || crash "N != 2"

exit 0
