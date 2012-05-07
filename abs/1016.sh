#!/bin/bash

var1=unset
previous=$var1

while echo "previous-variable = $previous"
      previous=$var1
      [ "$var1" != end ]
do
	echo "Input var #1 (end to exit)"
	read var1
	echo "variable #1 = $var1"
	echo
done

exit 0
