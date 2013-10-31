#!/bin/zsh

# Post HTML to SourceForge

# Input: SourceForge username:
SF_USER=$1
[[ ${SF_USER} == "" ]] && print "No SF_USER!" && exit 1

DIR=$( dirname $0 )
cd ${DIR}

HTML=( index.html manual.html )
IMG=( cmpi-connections.png )
FILES=( ${HTML} ${IMG} )
scp -p ${FILES} ${SF_USER},c-mpi@web.sourceforge.net:htdocs
