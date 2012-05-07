#!/bin/bash

dir1=/usr/local
dir2=/var/spool

pushd $dir1
echo "Now pwd=`pwd`"

pushd $dir2
echo "Now pwd=$(pwd)"

popd
echo "Now pwd=$(pwd)"

popd
echo "Now pwd=$(pwd)"
