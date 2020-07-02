#!/bin/bash

if [ "$EUID" -ne 0 ]
  then echo "Please run as root"
  exit
fi

DIR=$(pwd)

chmod +x bridge_manager.py

cat << EOT > /etc/systemd/system/bridge_manager.service
[Unit]
Description=Bridge Manager
Wants=network.target
After=syslog.target network-online.target

[Service]
Type=simple
WorkingDirectory=${DIR}
ExecStart=${DIR}/bridge_manager.py
Restart=on-failure
RestartSec=10
KillMode=process

[Install]
WantedBy=multi-user.target
EOT

systemctl daemon-reload
systemctl enable bridge_manager
systemctl start bridge_manager
