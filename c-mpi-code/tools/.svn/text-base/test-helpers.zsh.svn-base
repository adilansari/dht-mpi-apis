
# Sourced by tests for reusable functionality

col()
# Select columns from input without awk
{
  local L
  local -Z c
  local A
  while read L
  do
    A=( $( print ${L} ) )
    for c in ${*}
    do
      printf "${A[c]} "
    done
    printf "\n"
  done
}

USE_COMM_WORLD=$( grep USE_COMM_WORLD cmpi-config.h | col 3 )

crash()
{
  print $1
  if [ -f ${OUTPUT} ]
      then
      mv -v ${OUTPUT} ${OUTPUT}.failed
  else
      print "Not created: ${OUTPUT}"
  fi
  exit 1
}

tool_error()
{
  TOOL=$1
  ERR=$2
  if [[ ${ERR} != 0 ]]
  then
    print "${TOOL}: error: ${ERR}"
    kill ${DB_PID}
    kill ${BOMB_PID}
    crash "failed"
  fi
}

check_exits()
# Should be TOTAL "Normal exit."s
{
  TOTAL=$1
  OUTPUT=$2
  N=$( grep -c "Normal exit." ${OUTPUT} )
  (( N == TOTAL )) || crash "N != ${TOTAL}"
}

# Local variables:
# mode: sh
# End:
