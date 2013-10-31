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

  # Monolithic execution (3 nodes, 3 clients):

  ${LAUNCH} -n ${TOTAL} ${PROGRAM} -n ${NODES} >& ${OUTPUT}
  CODE=$?
  [[ ${CODE} == 0 ]] || crash "exit code was: ${CODE}"

else

  # KDA-2B execution:

  tools/startup_nodes.zsh $(( N-1 )) 100 &
  ${LAUNCH} -n 1 test/cmpi/test01.x > CLIENT.out
  sleep $(( N+15 ))

fi

# Should be TOTAL "Normal exit."s
check_exits ${TOTAL} ${OUTPUT}

exit 0
