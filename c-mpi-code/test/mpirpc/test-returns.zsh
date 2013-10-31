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

TOKENS=("result: ok"
        "result: (null)" )

for T in ${TOKENS}
do
  grep ${T} ${OUTPUT} > /dev/null || crash "Not found: ${T}"
done

exit 0
