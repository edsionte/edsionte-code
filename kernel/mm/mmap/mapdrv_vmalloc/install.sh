#! /bin/sh

/sbin/insmod ./map_driver.ko
major=$(awk '$2 ~ /^mapdrv$/ {print $1}' /proc/devices)
echo $major
mknod /dev/mapdrv0 c $major 0

