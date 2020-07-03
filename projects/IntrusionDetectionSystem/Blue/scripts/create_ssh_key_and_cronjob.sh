#!/bin/bash

# !This script was created for the use in the LICSTER Testbed!

# Script for generating and sharing a SSH-KEY with the HMI

logpath=/home/pi/gits/devices/hmi/LogReader/

argument1=$1 #user@ip

if [ "$argument1" == "-h" ]
	then
	  echo "Usage:"
	  echo "create_ssh_key_and_cronjob USER@IP"
	  echo "create_ssh_key_and_cronjob pi@192.168.0.20"
	  exit 0
fi

if ssh-keygen -t rsa -f ~/.ssh/id_rsa -P "" -C "Key for IDS -> HMI"; then 
	echo "Key generated"
else
	exit 1
fi

if ssh-copy-id "$argument1"; then
	echo "Key shared"
else
	echo  "! Key couldn't be shared !"
	echo "no cronjob was created"
	exit 1
fi

#a little script to create the cronjob to send the logs from snort to the hmi

if crontab -l | { cat; echo "* * * * * scp $logpath $argument1:/home/pi/gits/devices/hmi/LogReader/ > /dev/null 2>2&1"; } | crontab -; then
	echo "Cronjob was created"
else
	echo "no cronjob was created"
	exit 1
fi


