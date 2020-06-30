# Pcap

You can find under pcaps different pcaps which where taken from the testbed during different jobs.

1) Booting of the Testbed [pcap](pcaps/LICSTER_BOOTUP.pcapng)
2) Test-bed doing nothing [pcap](pcaps/LICSTER_ON_IDLE.pcapng)
3) Test-bed working [pcap](pcaps/LICSTER_ON_WORKING.pcapng)
4) Test-bed shutingdown [pcap](pcaps/LICSTER_SHUTDOWN.pcapng)
5) Attack with the conveyer_belt.py skript [pcap](pcaps/LICSTER_ATTACK_CONVEYOR_BELT_PY.pcapng)
6) FLoding of the remote io 192.168.0.51 [pcap](pcaps/LICSTER_HPING_FLOOD_51.pcapng)
7) Flooding of the remote io 192.168.0.52 [pcap](pcaps/LICSTER_HPING_FLOOD_52.pcapng)

# Setup for logfile transfer from IDS to HMI

Quick instruction on how to use the provided scripts.
1) run LICSTER-sharesshkey
2) run cronjob-scp

## LICSTER-sharesshkey [ssshk](scripts/licster-sharesshkey.sh)

This script creates a SSH-Key for the passwordless connection to the HMI. 
Just pass the user and the IP of the HMI as arguments.
For this example we have the HMI on *192.168.0.20* and the name of the user is *pi*

```shell
bash LICSTER-sharesshkey.sh pi@192.168.0.20
```

## cronjob-scp [cscp](scripts/cronjob-scp.sh)

This scripts only purpose is to create a cronjob which sends the log files created by snort to the hmi.
For this example we have the HMI on *192.168.0.20* and the name of the user is *pi*

```shell
bash cronjob-scp.sh
```
> This script needs editing if your default IP of the HMI and/or the default folder for the logs changes

### How it works

The whole purpose of these two scripts is to send the log-file which snorts creates to the HMI where it can be processed and used for the UI.
It starts with creating a ssh-key for passwordless ssh-connections between the IDS and the HMI. After the creation the key is shared with the given user and ip.
With this, we don't have to enter a password everytime the cronjob sends the log-files. The last part of the scripts creates a cronjob which
sends the log-files from snort to a specific folder at the HMI.

### Edit the script

Feel free to edit the scripts as you please. These scripts only exists to ease the configuration. Breaking the scripts will not result in any major errors.
