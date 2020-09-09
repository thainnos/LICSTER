#!/bin/bash
cd /home/pi/gits/LICSTER/devices/hmi/software
SECRET=`cat /dev/urandom | tr -dc '(\&\_a-zA-Z0-9\^\*\@' | fold -w ${1:-32} | head -n 1`
export SECRET_KEY=$SECRET
echo $SECRET_KEY > secret.key
gunicorn -w 4 -b 0.0.0.0:8080 "app:create_app()"
