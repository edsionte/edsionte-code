#!/bin/sh

module="globalmem"
device="globalmem"
mode="664"

/sbin/insmod ./$module.ko || exit 1

rm -f /dev/${device}

major=`awk "\\$2==\"$module\" {print \\$1}" /proc/devices`

mknod /dev/${device} c $major 0

group="lzel"
grep -q '^lzel:' /etc/group || group="root"

chgrp $group /dev/${device}
chmod $mode /dev/${device}
