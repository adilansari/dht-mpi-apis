#!/bin/zsh

# Be sure to make tests with D=1

# set -x

source tools/test-helpers.zsh

PROGRAM=$1
NODES=$2
CLIENTS=$3
shift 3
LAUNCH=${*}

OUTPUT=${PROGRAM%.x}.out
TOTAL=$(( NODES+CLIENTS ))

if (( USE_COMM_WORLD == 1 ))
then

  eval ${LAUNCH} -n ${TOTAL} ${PROGRAM} -n ${NODES} -p reps=10 >& ${OUTPUT}
  CODE=$?
  [[ ${CODE} == 0 ]] || crash "exit code was: ${CODE}"

else

  # KDA-2B execution:

  tools/startup_nodes.zsh $(( N-1 )) 100 &

  mpiexec -n 1 test/cmpi/test01.x > CLIENT.out

  sleep $(( N+15 ))

fi

# Should be TOTAL "Normal exit."s
N=$( grep -c "Normal exit." ${OUTPUT} )
(( N == TOTAL )) || crash "N != ${TOTAL}"

exit 0
