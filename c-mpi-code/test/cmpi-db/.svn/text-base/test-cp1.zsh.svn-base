#!/bin/zsh

# Be sure to make tests with D=1

#set -x

OUTPUT=$1
NODES=$2
CLIENTS=$3

[[ ${OUTPUT} == "" ]] && print "Bad args!" && exit 1

TOTAL=$(( NODES+CLIENTS ))

source tools/test-helpers.zsh

mpiexec -n ${TOTAL} bin/cmpi-db -n ${NODES} >& ${OUTPUT} &
DB_PID=${!}

tools/timebomb.zsh ${DB_PID} $(( TOTAL*40 )) ${OUTPUT} $0 &
BOMB_PID=${!}

sleep ${TOTAL}

echo "DATA" > test-cp1.input.data
echo "cmpi-cp insert" >& cmpi-cp.out
bin/cmpi-cp test-cp1.input.data dht://test-cp1 >>& cmpi-cp.out
if [[ $? != 0 ]]
then
  cat cmpi-cp.out
  rm -v cmpi-cp.out
  crash "cmpi-cp error!"
fi

echo "\n\ncmpi-cp retrieve" >>& cmpi-cp.out
bin/cmpi-cp dht://test-cp1 test-cp1.output.data >>& cmpi-cp.out
if [[ $? != 0 ]]
then
  cat cmpi-cp.out
  rm -v cmpi-cp.out
  crash "cmpi-cp error!"
fi

bin/cmpi-db-quit >& /dev/null
tool_error "cmpi-db-quit" ${?}
QUIT_PROCESS=$!

wait ${DB_PID}

kill ${BOMB_PID}

sleep ${TOTAL}

# Should be ${TOTAL} "Normal exit."s
check_exits ${TOTAL} ${OUTPUT}

rm -f cmpi-cp.out test-cp1.*.data

exit 0
