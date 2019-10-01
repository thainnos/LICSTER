#!/bin/bash
BINARY="build/stm32f767.bin"

echo "++++++++++++++++++++++++++++++++++++++++++++++++"
echo "+ Automatic ST-FLASHER by Matthias Niedermaier +"
echo "+ Flashing all devices detected by probe       +"
echo "++++++++++++++++++++++++++++++++++++++++++++++++"

if [ "$EUID" -ne 0 ]
  then echo "Please run as root"
  exit
fi

#Only for debug
#set -x

#Call st-info --probe to get devices
OUTPUT="$(st-info --probe)"

OUTPUT="$(echo "${OUTPUT}" | sed -n -e '/^ serial/p')"

#Use SED to get serial numbers
SERIALSLINE="$(echo "${OUTPUT}" | sed -e 's/[^0-9]//g')"

#Debug print found Serials
echo "Found the following devices:"
echo "${SERIALSLINE}"

# Split line into array to for loop
SERIALS=($SERIALSLINE)
#SERIALS+=("303637304646353734393533383637")
# Check for duplicate SN?
echo "++++++++++++++++++++++++++++++++++++++++++++++++"
echo "Check for duplicate serial numbers?"
printf 'Duplicate: %s\n' "${SERIALS[@]}"|awk '!($0 in seen){seen[$0];next} 1'

# Flashing every device connected/found
COUNT=0
for i in "${SERIALS[@]}"
do
	echo "++++++++++++++++++++++++++++++++++++++++++++++++"
	echo "Flashing Device ${COUNT}, with SN: ${i}"
	COUNT=$[$COUNT +1]
	set -x
	OUTPUT="$(st-flash --serial ${i} write ${BINARY} 0x8000000)"
	set +x
done

echo "++++++++++++++++++++++++++++++++++++++++++++++++"
echo "Totally flashed devices: ${COUNT}"
