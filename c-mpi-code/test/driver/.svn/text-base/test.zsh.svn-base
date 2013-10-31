#!/bin/zsh

OUTPUT=$1
NODES=$2
CLIENTS=$3
CMDS=$4
DRIVER_DIR=$5

TOTAL=$(( NODES+CLIENTS ))

source tools/test-helpers.zsh

bail()
{
    [[ ${BOMB_PID} != "" ]] && kill ${BOMB_PID}
    crash $1
}

# set -x

if [[ ${DRIVER_DIR} == "" ]]
then
  DRIVER_DIR="/tmp"
fi
CMPI_OPTS="-p driver_dir=${DRIVER_DIR}"

[[ ${LAUNCH} == "" ]] && bail "No LAUNCH specified!"

DRIVER=test/driver/test_driver.x
${LAUNCH} -n ${TOTAL} ${DRIVER} -n ${NODES} ${CMPI_OPTS} > ${OUTPUT} &
DRIVER_PID=${!}

tools/timebomb.zsh ${$} 30 ${OUTPUT} $0 &
BOMB_PID=${!}

sleep 3

source ${CMDS}

wait ${DRIVER_PID}
kill ${BOMB_PID}

check_exits ${TOTAL} ${OUTPUT}

return 0

# Local variables:
# sh-basic-offset: 2
# End:
