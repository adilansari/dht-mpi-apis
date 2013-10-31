#!/bin/zsh

PROG=$1
OUTPUT=$2

${LAUNCH} ${PROG} > ${OUTPUT}
if [[ $? != 0 ]]
then
  mv -v ${OUTPUT} ${OUTPUT}.failed
  exit 1
fi

return 0
