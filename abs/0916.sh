#!/bin/bash

: ${1?"Usage: $0 ARGUMENT"}

echo "These two lines echo only if command-line parameter geiven"
echo "command line parameter = \"$1\""

exit 0
