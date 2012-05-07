#!/bin/sh

module="globalmem"
device="globalmem"
mode="644"

major=`awk "\\$2==\"$device\" {print \\$1}" /proc/devices`
#echo $major

/sbin/rmmod $module || exit 1

if [ -c /dev/$device ]; then
	rm -f /dev/$device
fi

exit 0
