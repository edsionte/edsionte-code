#!/bin/bash

var0=0
LIMIT=10
 
while [ "$var0" -lt "$LIMIT" ]
do
	echo -n "$var0 "
	let "var0 += 1"
#var0=$(($var0+1))

done
echo
exit 0
