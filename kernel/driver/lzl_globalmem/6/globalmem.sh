#!/bin/sh

module="globalmem"
device="globalmem"
mode="664"

/sbin/insmod ./$module.ko || exit 1

rm -f /dev/${device}[0-1]

major=`awk "\\$2==\"$module\" {print \\$1}" /proc/devices`

mknod /dev/${device}0 c $major 0
mknod /dev/${device}1 c $major 1

group="lzel"
grep -q '^lzel:' /etc/group || group="root"

chgrp $group /dev/${device}[0-1]
chmod $mode /dev/${device}[0-1]
