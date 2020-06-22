#!/bin/bash
cd /home/pi/gits/LICSTER/devices/hmi/software
gunicorn -w 4 -b 0.0.0.0:8080 "app:create_app()"
