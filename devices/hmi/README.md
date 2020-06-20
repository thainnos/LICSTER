# Low-cost ICS Testbed - HMI

## Installation

### Clone the repository

Create the folder "gits" in the home path and switch to it.

```
mkdir gits
cd gits
```

Clone the git of the low cost testbed.

```
git clone https://github.com/hsainnos/LICSTER.git
```

Change into the correct folder.

```
cd ~/gits/LICSTER/devices/hmi
```


### Install all dependencies


#### Install unclutter to disable the mouse pointer on the HMI.

```
sudo apt install unclutter npm
```

#### Install the python dependencies.

```
sudo python3 -m pip install pymodbus WTForms Flask-WTF gunicorn
```

#### Install bootstrap and jQuery.

```
cd ~/gits/LICSTER/devices/hmi/software/app/static
npm install package.json
```

## Setup

#### Initialize the Web Application

Initialize the database.

```
cd ~/gits/LICSTER/devices/hmi/software
flask init-db
```

Copy the autostart file to its correct place.

```
cd ~/gits/LICSTER/devices/hmi/
mkdir -p ~/.config/lxsession/lxde-pi/
cp autostart ~/.config/lxsession/lxde-pi/autostart
```

Make the startscript executable.

```
cd ~/gits/LICSTER/devices/hmi/
chmod +x startscript.sh
```

#### Enable Emails if the Intrusion Detection System finds something

Open the crontab file as root:

```
sudo nano /etc/crontab
```

Add the following lines at the end of the file.

```
* * * * *   root cd /home/pi/gits/LICSTER/devices/hmi/LogReader && ./minutely
#
```

Press __Strg__ + __O__ to save the file. Then press __Strg__ + __X__ to leave the editor. 

#### Configure the IP Adress of the HMI.

```
sudo nano /etc/network/interfaces
```

The file should look like this at the end. With this, you will have a static IP and if possible also one from DHCP.

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
    address 192.168.0.20
    netmask 255.255.255.0

```


## How can I?

#### Change my HMI password

```
cd ~/gits/LICSTER/devices/hmi/software/
flask change-hmi-password
```
