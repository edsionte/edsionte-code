#!/bin/sh

module="globalmem"
device="globalmem"
mode="644"

major=`awk "\\$2==\"$device\" {print \\$1}" /proc/devices`
#echo $major

/sbin/rmmod $module || exit 1

if [ -c /dev/${device}0 ]; then
	rm -f /dev/${device}0
fi
if [ -c /dev/${device}1 ]; then
	rm -f /dev/${device}1
fi

exit 0
