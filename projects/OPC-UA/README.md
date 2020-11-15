# OPC-UA Integration 
*IMPORTANT*: Documentation incomplete 

## Installation

Be sure, that LICSTER is connected to the Internet and you have access to it.
Connect to the openPLC Raspberry Pi.
```sh
ssh pi@192.168.0.30
```

Checkout the current GIT.
```sh
cd ~\gits\LICSTER
git pull
```

Change into the OPC-UA folder to build and flash the remote IOs.
```
cd ~\gits\LICSTER\projects\OPC-UA\software
make -j8
sudo ./flashall
```

Install dependencies.
```sh
sudo apt install python3-pip libxslt1.1
sudo python3 -m pip install opcua
```
