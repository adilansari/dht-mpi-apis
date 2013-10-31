#!/bin/bash

rm -fv config.cache

mkdir -p maint/config

if [[ ! -f configure || configure.ac -nt configure ]]
then
  echo "regenerating..."
else
  echo "CMPI configure is up-to-date"
  exit 2
fi

if aclocal -I ./maint/config && autoheader && autoconf ; then
  echo "CMPI configure script successfully regenerated"
else
  echo "Some part of configure regeneration failed!"
  exit 1
fi

exit 0
