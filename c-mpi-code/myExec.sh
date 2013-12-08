#!/bin/bash

START=$(date +%s)
while getopts ":s:p:" opt; do
  case $opt in
    s)
	CLIENTS=$((OPTARG+OPTARG)) >&2
	echo "Startup test was triggered, for $OPTARG nodes and $OPTARG clients" >&2
	mpiexec.hydra -n $CLIENTS test/cmpi/test-startup.x -n $OPTARG >&2
	END=$(date +%s) >&2
	TIME=$(( $END - $START )) >&2
	echo "" >&2
	echo "Time taken to setup a DHT of $OPTARG nodes is $TIME seconds" >&2
	echo "$OPTARG $TIME" >> result/startup_time.out
      ;;
    p)
      echo "-p was triggered, Parameter: $OPTARG" >&2
      ;;
    \?)
      echo "Invalid option: -$OPTARG" >&2
      exit 1
      ;;
    :)
      echo "Option -$OPTARG requires an argument." >&2
      exit 1
      ;;
  esac
done
