# Low-cost ICS Testbed - HMI

Install unclutter to disable mouse pointer on the HMI.
```zsh
sudo apt install unclutter npm
```

Install dependencies for LICSTER/python3.
```zsh
sudo python3 -m pip install pymodbus WTForms Flask-WTF gunicorn 
```

Create the folder "gits" in the home path and switch to it.
```zsh
mkdir gits
cd gits
```

Clone the git of the low cost testbed.
```zsh
git clone https://github.com/hsainnos/LICSTER.git
```

Change into the correct folder.
```zsh
cd ~/gits/LICSTER/devices/hmi
```

Initialize the database. This will create:
	An admin user with the name and password admin
	The default user for the hmi with the password 7255
```zsh
cd ~/gits/LICSTER/devices/hmi/software
flask init-db
```

Copy the autostart file to its correct place.
```zsh
mkdir -p ~/.config/lxsession/lxde-pi/
cp autostart ~/.config/lxsession/lxde-pi/autostart
```

Install bootstrap and jQuerry.
```zsh
cd ~/gits/LICSTER/devices/hmi/software/static
npm install package.json
```

Configure IP Adress of the HMI.
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
    address 192.168.0.20
    netmask 255.255.255.0

```
