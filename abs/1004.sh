#!/bin/bash

for file in *
do
	ls -l "$file"
done

for a
do
	echo -n "$a "
done
echo
exit 0
