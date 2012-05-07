#! /bin/sh

/sbin/insmod ./mapdrv_kmalloc.ko

major=$(awk '$2 ~ /mapdrv_k/ {print $1}' /proc/devices)
echo $major
mknod /dev/mapdrv_k c $major 0

