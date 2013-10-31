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

# Should only be 3 calls to handle_test()...
N=$( grep -c "handle_test(args=" ${OUTPUT} )
(( N == 3 )) || crash "N != 3"

TOKENS=("handle_test(args=(null))"
        "handle_test(args=)"
        "handle_test(args=alpha)" )

for T in ${TOKENS}
do
  grep ${T} ${OUTPUT} > /dev/null || crash "Not found: ${T}"
done

exit 0
