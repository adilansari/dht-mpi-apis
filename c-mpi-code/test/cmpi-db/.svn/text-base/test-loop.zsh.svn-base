#!/bin/zsh

# Be sure to make tests with D=1
# Produces output in test-loop.out and tool.out

# set -x

OUTPUT=$1
NODES=$2
CLIENTS=$3

TOOL_OUT=tool.out
INPUT_DATA=input.data
OUTPUT_DATA=output.data

TOTAL=$(( NODES+CLIENTS ))

LOOPS=10

source tools/test-helpers.zsh

mpiexec -n ${TOTAL} bin/cmpi-db -n ${NODES} >& ${OUTPUT} &
DB_PID=${!}

tools/timebomb.zsh ${DB_PID} $(( TOTAL*10 + LOOPS*4 )) ${OUTPUT} $0 &
BOMB_PID=${!}

sleep ${TOTAL}

print > ${TOOL_OUT}
for ((i=0 ; i<LOOPS ; i++))
do
  echo "DATA_${i}" > ${INPUT_DATA}
  echo "cmpi-cp insert" >>& ${TOOL_OUT}
  bin/cmpi-cp ${INPUT_DATA} dht://test-${i} >>& ${TOOL_OUT}
  tool_error "cmpi-cp" ${?} >>& ${TOOL_OUT}

  echo "\n\ncmpi-cp retrieve" >>& ${TOOL_OUT}
  bin/cmpi-cp dht://test-${i} ${OUTPUT_DATA} >>& ${TOOL_OUT}
  tool_error "cmpi-cp" ${?} >>& ${TOOL_OUT}
  echo "\n read: " >>& ${TOOL_OUT}
  cat ${OUTPUT_DATA} >>& ${TOOL_OUT}
  echo "\n" >>& ${TOOL_OUT}
done

echo "quitting" >>& ${TOOL_OUT}
bin/cmpi-db-quit >>& ${TOOL_OUT}
tool_error "cmpi-db-quit" ${?} >>& ${TOOL_OUT}
echo "quit" >>& ${TOOL_OUT}

# QUIT_PROCESS=${!}

wait ${DB_PID}

kill ${BOMB_PID}

sleep ${TOTAL}

check_exits ${TOTAL} ${OUTPUT}

exit 0
