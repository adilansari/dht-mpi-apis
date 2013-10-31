#!/bin/zsh 

M=$1
N=$2 

[[ $M == "" ]] && print "Need test number!" && exit 
[[ $N == "" ]] && N=4

# tools/startup_nodes.zsh ${N} 100 & 

# sleep $(( N+15 )) 

# KDA-1 execution: 
mpiexec -n ${N} test/cmpi/test${M}.x -n $(( N-1 )) >& test/cmpi/test${M}.out 

