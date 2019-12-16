# Programmable Logic Controller (PLC)
This section will briefly describe what a PLC is and it is a key component for production environments and then focus on
the setup of the Soft-PLC used for this testbed and the program that it executes. This page is structured as follows:

1. [PLC Basics](#plc-basics)
    1. [What is a PLC?](#what-is-a-plc?)
    2. [What is a Soft-PLC?](#what-is-a-soft-plc?)
2. [The PLC as the heart of our testbed](#the-plc-as-the-heart-of-our-testbed)
    1. [Setup](#setup)
    2. [Program](#program)

## PLC Basics
As the successor of traditional relay circuits, which are basically switches that can be toggled electrically, the PLC
has become indispensable for our industrialized world. They control production plants as well as critical
infrastructures such as water and power supply. The PLC is the interface between the virtual world of software and the
physical world of signals and circuits. The fact that is is deployed almost everywhere makes it the Achilles tendon of
many machines, plants and infrastructures.

### What is a PLC?
Typically, a PLC is designed to endure even in rough industrial environments. It has a small form factor so it can be
mounted on top-hat-rails. A PLC consists out of a CPU, memory and said inputs and outputs. Further it has a series of
digital and or analogue inputs and outputs that are controlled by its internal logic, i.e. the implemented program.
These programs are very low level an run close to the actual hardware. Without the need for a full-fledged operating
system, a PLC does not requires less computing power. Especially the lack of a scheduler, a central component of
operating systems that splits the processing power between all processes, allows PLCs to meet industrial real-time
requirements.

### What is a Soft-PLC?
A regular PLC is software and hardware molded together inseparably. A Soft-PLC is the software of a PLC on its own, which
then can be installed on various hardware platforms. Depending on the underlying hardware, the Soft-PLC has more or
fewer inputs and outputs, computing power or other interfaces. This makes PLCs much more flexible and more affordable
due to open-source software and inexpensive hardware of one's choosing. 

## The PLC as heart of the testbed
One of the fundamental requirements for this project is to keep all components open-source and affordable. Out of these
reasons we decided to use a Raspberry Pi as hardware platform to host the Soft-PLC
[OpenPLC](https://openplcproject.com). 

### Setup
Install a raspbian image on a SD Card and get a shell.

Configure IP Adress of the OpenPLC Raspberry Pi.
```zsh
sudo nano /etc/network/interfaces
```

The file should look like this at the end.
With this, you will have a static IP and if possible also one from DHCP. 
```
# interfaces(5) file used by ifup(8) and ifdown(8)

# Please note that this file is written to be used with dhcpcd
# For static IP, consult /etc/dhcpcd.conf and 'man dhcpcd.conf'

# Include files from /etc/network/interfaces.d:
source-directory /etc/network/interfaces.d

auto lo eth0 eth0:0
iface lo inet loopback

iface eth0 inet dhcp

iface eth0:0 inet static
    address 192.168.0.30
    netmask 255.255.255.0

```

Create git folder and change into it.
```zsh
mkdir gits
cd gits
```

Cloning the OpenPLCv3 git repository.
```zsh
git clone https://github.com/thiagoralves/OpenPLC_v3.git
```

Change into the OpenPLCv3 folder.
```zsh
cd OpenPLC_v3/
```

Install OpenPLCv3 for Linux.
```zsh
./install.sh linux
```
### Program
Open your browser and go to ![http://192.168.0.30:8080/](http://192.168.0.30:8080/).
The username and password is openplc:openplc.

<table align="center"><tr><td align="center" width="9999">
<img src="images/login.png" width=70%></img>
</td></tr></table>

After this, configure the two slave devices as following:

<table align="center"><tr><td align="center" width="9999">
<img src="images/slave_device01.png" width=70%></img>
</td></tr></table>

<table align="center"><tr><td align="center" width="9999">
<img src="images/slave_device02.png" width=70%></img>
</td></tr></table>

Then, the ST user program can be uploaded and compiled:

<table align="center"><tr><td align="center" width="9999">
<img src="images/upload.png" width=70%></img>
</td></tr></table>

Select the user program:
<table align="center"><tr><td align="center" width="9999">
<img src="images/upload.png" width=70%></img>
</td></tr></table>

Select the user program:
<table align="center"><tr><td align="center" width="9999">
<img src="images/upload2.png" width=70%></img>
</td></tr></table>

Give the user program a name:
<table align="center"><tr><td align="center" width="9999">
<img src="images/name.png" width=70%></img>
</td></tr></table>

Select "Upload program" and the ST program is compiled by OpenPLC:
<table align="center"><tr><td align="center" width="9999">
<img src="images/compile.png" width=70%></img>
</td></tr></table>

Now make the settings of the OpenPLC:
<table align="center"><tr><td align="center" width="9999">
<img src="images/settings.png" width=70%></img>
</td></tr></table>

Press "Start PLC":
<table align="center"><tr><td align="center" width="9999">
<img src="images/start.png" width=70%></img>
</td></tr></table>

### Structure
<table align="center"><tr><td align="center" width="9999">
<img src="images/list-network.png" width=70%></img>
</td></tr></table>

