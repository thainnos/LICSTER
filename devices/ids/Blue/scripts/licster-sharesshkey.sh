#!/bin/bash

# !This script was created for the use in the LICSTER Testbed!

# Script for generating and sharing a SSH-KEY with the HMI

if [ "$1" == "-h" ]
	then
	  echo "Usage:"
	  echo "licster-sharesshkey USER@IP"
	  echo "licster-sharesshkey hmi@192.168.0.20"
	  exit 0
fi

ssh-keygen -t rsa -f ~/.ssh/id_rsa -P "" -C "Key for IDS -> HMI"

ssh-copy-id "$1"

