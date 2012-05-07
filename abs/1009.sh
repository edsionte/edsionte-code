#!/bin/bash

dirctory=/usr/bin/
fstring="root"

for file in $(find $directory -type f -name '*' | sort)
do
	strings -f $file | grep "$fstring" | sed -e "s%$dirctory%%"
done

exit 0
