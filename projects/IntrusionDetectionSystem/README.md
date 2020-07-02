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

## Content

* [How to work with snort](snort/README.md)
* [Rules](snort/rules/README.md)
* [Pcap](Blue/README.md)
* [Integration WEB HMI]()

## Authors

* **Janis Shickram**
* **Eric Hoffmann**
* **Michael Janzer**

## License

* [Snort](https://www.snort.org/license)

