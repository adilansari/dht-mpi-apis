#!/bin/zsh

# Check that MPD is up

MPD_COMMAND=$1

${MPD_COMMAND} > /dev/null
if [[ $? != 0 ]]
then
  ${MPD_COMMAND}
  exit 1
fi

return 0
