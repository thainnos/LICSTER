#!/bin/bash

#a little script to create the cronjob to send the logs from snort to the hmi

crontab -l | { cat; echo "* * * * * scp /home/pi/gits/devices/hmi/LogReader/ pi@192.168.0.20:/home/pi/gits/devices/hmi/LogReader/ > /dev/null 2>2&1"; } | crontab -
