#!/bin/bash
while [ $# -gt 0 ]
do 
	key=$1
	case $key in
		-e)
		extension=$2
		shift 2
		echo $extension
		;;
		
		-c)
		compress=yes
		echo $compress
		shift
		;;

		-*)
		echo "ERROR option $1 not found."
		shift
		exit 1
		;;

		*)
		echo "PROCESS FILE: $1"
		shift
		;;


	esac
done
echo DONE
echo $extension
echo $compression
