# Intrusion Detection System

The following Project should help get to know how an Intrusion Detection system
works and how it can be implemented into a working environment. It is part of 
its parent project "Low-cost ICS Testbed". The Project is built with snort as 
ids system. It contains a configuration guide so that it works with LICSTER, 
recommended hardware and how to write rules.

## Getting started

These instructions will get you a copy of the project up and running on your 
local machine for development and testing purposes.

### Prerequisites

It is recommended that the system where snort (or if you want to use another
system) is going to be installed, to be a separate system. The reason for that 
is that most ids have high resource requirements. And if you are running
it on your main system it will slow it down or crash the system altogether. In 
industrial systems most ids systems run on separate servers. Because we are 
using it only in a test environment, a Raspberry Pi is enough (A Raspberry Pi 3 
should be enough, we used a Raspberry Pi 4).

| What                        | Price        | Link                                                                                                   |
| --------------------------- | ------------ | ------------------------------------------------------------------------------------------------------ |
| Raspberry Pi 3              |  34 Euro     | [Amazon](https://amzn.to/2UMnFUS)                                                                      |
| Raspberry Pi 4    4GB RAM   |  60 Euro     | [Amazon](https://amz.run/3MOL)|

For the operating system you can use most of the well-known Linux distributions. 
We recommend [Raspbian](https://www.raspberrypi.org/downloads/raspbian/) for the 
Raspberry Pi.

### Installing

A step by step series of examples that tell you how to get snort running.
You can install two different versions of snort ‘snort 2’ or ‘snort 3’.

**Snort 2**

Run your normal installation process for a new software.

Raspbian/Debian
```
sudo apt install snort
```
Now it should ask you to set the home net, use the ip range you set for the 
testbed (192.168.0.0/24).

Now when you type
```
sudo snort
```
You should see
```
Running in packet dump mode

        --== Initializing Snort ==--
Initializing Output Plugins!
pcap DAQ configured to passive.
Acquiring network traffic from "eth0".
Decoding Ethernet

        --== Initialization Complete ==--

   ,,_     -*> Snort! <*-
  o"  )~   Version 2.9.7.0 GRE (Build 149) 
   ''''    By Martin Roesch & The Snort Team: http://www.snort.org/contact#team
           Copyright (C) 2014 Cisco and/or its affiliates. All rights reserved.
           Copyright (C) 1998-2013 Sourcefire, Inc., et al.
           Using libpcap version 1.8.1
           Using PCRE version: 8.39 2016-06-14
           Using ZLIB version: 1.2.11
[...]
```
If this appears, you know everything is alright. You have to pay attention that 
snort is run as root.

**Snort 3**

This Version of snort is the newer one but is more complicated to install. For this
Installation  the easiest way is to go to [snort](https://www.snort.org/documents) 
and chose under Snort Setup Guides the installation guide you need. You can also go to 
their git project [snort](https://github.com/snort3/snort3) and folow the instructions which are writen their.

## Running snort

The snort.conf file, which you are mostly going to work with, can be found in 
the snort folder. The snort.conf is already configured to use the custom rules 
for the LICSTER test bed and the Home-Net 192.168.0.0/24.

There are many different modes with which you can run snort, we will only
explain the most helpful modes. (For more information read the 
[snort Docommentation](https://snort-org-site.s3.amazonaws.com/production/document_files/files/000/000/249/original/snort_manual.pdf))

### Different Modes

Using a custom snort.conf
````
sudo snort -c /path/to/LICSTER/projects/IntrusionDetectectionSystem/snort/snort.conf
````
Running snort so that it logs to a directory.
````
sudo snort -l ./path/to/log/file -b
````
Running snort so it shows the command in the terminal
````
sudo snort -A console
````
Telling snort to use a pcap
````
sudo snort -r /path/to/LICSTER/projects/IntrusionDetectectionSystem/Blue/pcaps/file
````
Normally you use a combination of different modes
````
sudo snort -l ./path/to/log/file -b -A console -c /path/to/LICSTER/projects/IntrusionDetectectionSystem/snort/snort.conf -r /path/to/LICSTER/projects/IntrusionDetectectionSystem/Blue/pcaps/file
````
## Rules system

Our custom rules for snort are already included in the snort.conf, so you don't have to change anything.
If you want to write your own rules see [here](snort/rules/README.md).

## Integrating Snort to the WEB HMI

To integrate snort to the HMI, so that the HMI shows alerts if snort throws one, you have to create a cronjob,
which sends the logs to the HMI. We made two bash-scripts, the [licster-sharesshkey.sh](Blue/scripts/licster-sharesshkey.sh)
and the [cronjob-scp.sh](Blue/scripts/cronjob-scp.sh), which make it easier to setup. The [licster-sharesshkey.sh](Blue/scripts/licster-sharesshkey.sh)
script creates an ssh-key and shares the public key with the HMI. The [cronjob-scp.sh](Blue/scripts/cronjob-scp.sh) script creates a cronjob, so that every minute the logs
are transferred, using scp, to the HMI. You should run the ssh-key script first and then the cronjob script.
To run them type:
````
$ ./licster-sharesshkey.sh
````
and
````
$ ./cronjob-scp.sh
````

## Content

* [Rules](snort/rules/README.md)
* [Pcap](Blue/README.md)
* [Integration WEB HMI](Blue/scripts/README.md)

## Authors

* **Janis Shickram**
* **Eric Hoffmann**
* **Michael Janzer**

## License

* [Snort](https://www.snort.org/license)

