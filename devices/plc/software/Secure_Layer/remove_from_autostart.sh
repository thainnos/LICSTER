#!/bin/bash

if [ "$EUID" -ne 0 ]
  then echo "Please run as root"
  exit
fi

systemctl stop bridge_manager
systemctl disable bridge_manager
rm /etc/systemd/system/bridge_manager.service
systemctl daemon-reload
