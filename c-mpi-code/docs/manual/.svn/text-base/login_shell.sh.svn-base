#!/bin/sh

# Allocate 6 compute nodes
qsub -t 1-6 ...

# List node host names to file for mpiexec
qstat | something > hosts

# Create some initial conditions
create_initial > input

# Launch the DHT (12 processes on 6 nodes)
mpiexec.hydra -f hosts -n 12 bin/cmpi-db -n 6 &

# Spawn client scripts
# (Could use Falkon here?)
id=0
total=$( wc -l hosts )
for host in $( cat hosts )
do
  ssh ${host} client_script.sh $(( id++ )) ${total} &
done

wait
