#!/bin/bash

NO_ARGS=0
E_OPTERROR=65

if [ $# -eq "$NO_ARGS" ]
then
	echo "Usage:`basename $0` options (-mnopqrs)"
	exit $E_OPTERROR
fi

while getopts ":mnopq:rs" option
do
	case $option in
	m)
		echo "option -m- [OPTIND=${OPTIND}]";;
	n|o)
		echo "option -$option- [OPTIND=${OPTIND}]";;
	p)
		echo "option -p- [OPTIND=$OPTIND]";;
	q)
		echo "option -q- witch argument $OPTARG";;
	r|s)
		echo "option -$option- [OPTIND=$OPTIND]";;
	*)
		echo "Unimplememted option chose.";;
	esac
done

shift $(($OPTIND-1))
