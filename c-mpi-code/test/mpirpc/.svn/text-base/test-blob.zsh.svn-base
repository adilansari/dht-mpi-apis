#!/bin/zsh

# Be sure to make tests with D=1

source tools/test-helpers.zsh

# set -x

PROGRAM=$1
OUTPUT=${PROGRAM%.x}.out
shift
LAUNCH=${*}

eval ${LAUNCH} -n 2 ${VALGRIND} ${PROGRAM} > ${OUTPUT} 2>&1
CODE=$?
[[ ${CODE} == 0 ]] || crash "exit code was: ${CODE}"

TOKENS=("asserts ok"
        "result: (null)" )

for T in ${TOKENS}
do
  grep ${T} ${OUTPUT} > /dev/null || crash "Not found: ${T}"
done

# Should be 2 "Normal exit."s
N=$( grep -c "Normal exit." ${OUTPUT} )
(( N == 2 )) || crash "N != 2"

exit 0
