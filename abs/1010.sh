#!/bin/bash

directory=/home/edsionte/

for file in "$(find $directory -type l)"
do 
	echo "$file"
done | sort
