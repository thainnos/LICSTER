# Low-cost ICS Testbed - HMI

## Usage of the Raspberry Pi Release Images

https://github.com/hsainnos/LICSTER/releases

## Installation (Not necessary if Releases are used)

### Running Raspberry Pi with Desktop

Recommended and tested with Raspberry Pi OS (32-bit) with desktop on a Raspberry Pi 3.

### Clone the repository

Create the folder "gits" in the home path and switch to it.

```
mkdir gits
cd gits
```

Install git software on the Raspberry Pi
```
sudo apt install git
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
sudo apt install unclutter npm python3-pip xinit lxde-core lxterminal lxappearance lightdm
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
mkdir -p ~/.config/lxsession/LXDE-pi/
cp autostart ~/.config/lxsession/LXDE-pi/autostart
```

Make the startscript executable.

```
cd ~/gits/LICSTER/devices/hmi/
chmod +x startscript.sh
```

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

## Troubleshooting

#### Disconnected

The HMI shows, that it is disconnected even though everything is connected as it should be. This is not a problem of the HMI, but of the PLC. 

To fix this problem you can either flash the SD card of the PLC again or remove the gits repository from the PLC and follow the installation instructions for the PLC.

#### Screen rotation

Edit the boot configuration file of the Raspberry Pi.
```
sudo nano /boot/config.txt
```

Add the following line at the end of the file.
```
lcd_rotate=2
```


#### Power warning

Edit the boot configuration file of the Raspberry Pi.
```
sudo nano /boot/config.txt
```

Add the following line at the end of the file.
```
avoid_warnings=1
```

#### Auto login problem

Start the Raspberry Pi configuration screen:

```
sudo raspi-config
```

Select "3 Boot Options" and "B4 Desktop Autlogin Desktop GUI, automatically logged in as 'pi' user".
Exit and save.


