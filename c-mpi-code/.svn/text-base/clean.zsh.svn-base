#!/bin/zsh

# Cleanup before SVN operations
# Use ./config.status to get going again after this

MODULES=( $( print **/module.mk ) )
if [[ ${#MODULES} > 0 ]]
then
  make clean
  rm -fv ${MODULES}
  rm -fv **/*.tmp
  rm Makefile
fi

print "cleaned"
