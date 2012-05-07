#!/bin/bash

module="scull"
device="scull"

/sbin/insmod ./$module.ko || exit 1

rm -f /dev/${device}[0-3]


