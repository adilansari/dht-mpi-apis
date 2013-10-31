#!/bin/sh

id=$1
nodes=$2

for (( i=0 ; i<10 ; i++ ))
do
  # Do some useful work
  do_work ${id} ${nodes} < input > output
  # Post the results to the DHT
  key=output_${id}_${i}
  cmpi-cp output dht://${key}
  # Read a neighbor's result
  key=output_$(( id-1 % nodes ))_${i}
  cmpi-cp dht://${key} input
done
