#!/bin/bash

#这种方法可以检查某些环境变量是否被设置
#: ${HOSTNAME?} ${USER?} ${HOME?} ${MAIL?}  

echo 
echo "Name of the machine is $HOMENAME"
echo "You are $USER"
echo "Your home directroy is $HOME"
echo "Your mail INBOX is located in $MAIL"
echo
echo "If you are reading this message,"
echo "critical environmental variables have been set"
echo
echo

ThisVariable=Value-of-ThisVariable
: ${ThisVariable?}
echo "Value of ThisVariable is $ThisVariable"
echo 
echo

: ${ZZXy23AB?"ZZXy23AB has not been set"}

