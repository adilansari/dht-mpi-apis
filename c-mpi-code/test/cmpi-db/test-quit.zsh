#!/bin/zsh

# Be sure to make tests with D=1

# set -x

OUTPUT=$1
NODES=$2
CLIENTS=$3

TOTAL=$(( NODES+CLIENTS ))

source tools/test-helpers.zsh

mpiexec -n ${TOTAL} bin/cmpi-db -n ${NODES} >& ${OUTPUT} &
MPI_PROCESS=$!

sleep ${TOTAL}

bin/cmpi-db-quit

sleep ${TOTAL}

check_exits ${TOTAL} ${OUTPUT}

exit 0
