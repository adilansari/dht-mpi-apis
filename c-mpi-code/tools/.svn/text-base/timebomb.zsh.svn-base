#!/bin/zsh

TARGET=$1
DELAY=$2
DATA=$3
shift 3
LABEL=${*}

sleep ${DELAY}

print "timebomb killing: ${LABEL} PID: ${TARGET}"

mv -v ${DATA} ${DATA}.bombed

sleep 1

kill -9 ${TARGET}
