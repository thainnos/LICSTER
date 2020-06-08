# Secure Layer
This software component manages modbus connections wrapped in TLS.

1. [TLS](#tls)
   1. [What is TLS?](#what-is-tls)
   2. [How can LICSTER utilize TLS?](#how-can-licster-utilize-tls)
2. [Setup](#setup)
   1. [Creating Certificates](#creating-certificates)
   2. [Remote IO](#remote-io)
   3. [OpenPLC](#openplc)
3. [Bridge Manager](#bridge-manager)
   1. [What is the Bridge Manager?](#what-is-the-bridge-manager)
   2. [Running the Bridge Manager](#running-the-bridge-manager)
   3. [Removing the Bridge Manager](#removing-the-bridge-manager)
   4. [Additional configuration (Optional)](#additional-configuration-optional)

## TLS
### What is TLS?
Transport Layer Security, and its now-deprecated predecessor, Secure Sockets Layer, are cryptographic protocols designed to provide communications security over a computer network. Several versions of the protocols find widespread use in applications such as web browsing, email, instant messaging, and voice over IP.

### How can LICSTER utilize TLS?
By wrapping the modbus connection inside a TLS connection, ensure the authenticity of the modbus server we connect to, prevent other parties from eavesdropping the unencrypted communication, but also prevent MITM attacks!

## Setup
In order to make use of the TLS protocol we have to set up all our modbus components to use TLS:

### Creating Certificates
In order to make use of TLS we need certificates. Instead of using a public certificate authority (CA) to sign our certificates we create one local CA known as LICSTER. All devices presenting certificates signed by this local CA are to be seen as trustworthy.  
To create a new CA together with all the signed certificates do the following:

**Warning:** This will replace all previously generated certificates (including the CA)!

```sh
cd $HOME/LICSTER/tools
chmod +x cert_write create_new_certs_with_ca.sh gen_key
./create_new_certs_with_ca.sh
```

### Remote IO
[Read this](/devices/remote_io/software/README.md#building-tls-optional) for information on how to set up the Remote IO for TLS.

### OpenPLC
[Read this](/devices/plc/README.md#running-the-openplc-in-tls-mode) for information on how to set up the OpenPLC for TLS.

## Bridge Manager
### What is the Bridge Manager
The Bridge Manager is the application that creates, manages and restarts these TLS wrapped modbus connections. It is written in Python.

### Running the Bridge Manager
Change into the Secure_Layer folder of the software section inside the PLC:
```sh
cd $HOME/LICSTER/devices/plc/software/Secure_Layer
```

Start the Bridge Manager on boot:
```sh
chmod +x add_to_autostart.sh
sudo ./add_to_autostart.sh
```

### Removing the Bridge Manager
Change into the Secure_Layer folder of the software section inside the PLC:
```sh
cd $HOME/LICSTER/devices/plc/software/Secure_Layer
```

Disable and remove the Bridge Manager from the boot procedure:
```sh
chmod +x remove_from_autostart.sh
sudo ./remove_from_autostart.sh
```


### Additional configuration (Optional)
The Bridge Manager has a few options that can be configured. For more inforation check out its [configuration file](config.ini)