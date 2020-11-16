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
cd ~/gits/LICSTER/
git pull
```

Change into the OPC-UA folder to build and flash the remote IOs.
```
cd ~/gits/LICSTER/projects/OPC-UA/software/
make -j8
sudo ./flashall.sh
```

Install dependencies for the proxy.
```sh
sudo apt install python3-pip libxslt1.1
sudo python3 -m pip install opcua pymodbus
```

## Reconfigure openPLC

Start your browswer and go to http://192.168.0.30:8080/ and login to openPLC.
Delte the current program by clicking on it.
Upload the new programm under (LICSTER/projects/OPC-UA/software/machine.st) to the PLC.

## Run the proxy

Connect to the Raspberry Pi, which is running openPLC.
```sh
ssh pi@192.168.0.30
```

Change into the software folder of the OPC-UA project.
```sh
cd ~/gits/LICSTER/projects/OPC-UA/software
```

Run the proxy, which handles the communication between modbus/TCP and OPC-UA.
```sh
./proxy.py
```

## Test the setting
On your webbrowser go to http://192.168.0.20:8080/ and make some orders.

## OPC-UA client GUI
It is also possible to connect to the remote IOs, with your Linux computer.

Install the OPC-UA client software.
```sh
sudo apt install opcua-client
```

Open the software.
```sh
opcua-client
```

The connection URL is the following for remote IO 1: opc.tcp://admin:admin@192.168.0.51:4840

<table align="center"><tr><td align="center" width="9999">
<img src="images/opcuaclient.png" width=70%></img>
</td></tr></table>
