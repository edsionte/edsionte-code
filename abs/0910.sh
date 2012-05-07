#!/bin/bash

#在字符数少于45的行后面添加一个空行
MINLEN=45

while read line
do
	echo "$line"
	len=${#line}
	if [ "$len" -lt "$MINLEN" ]
		then echo
	fi
done

exit 0
