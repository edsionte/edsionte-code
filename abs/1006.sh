#!/bin/bash

NUMBERS="9 7 3 8 37.53"

for number in `echo $NUMBERS`
do
	echo -n -e "$number\t"
done

echo
exit 0
