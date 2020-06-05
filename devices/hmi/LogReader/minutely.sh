#!/bin/bash

FILEPATH="~/gits/LICSTER/devices/hmi/LogReader/snort-datei"
EXECUTEPATH="~/gits/LICSTER/devices/hmi/LogReader/snort.py"

SYSTEMTIME=`date "+%s"`
FILETIME=`date -r $FILEPATH "+%s"`

if (( $SYSTEMTIME - 60 < $FILETIME ))
then
	python3 $EXECUTEPATH
fi

