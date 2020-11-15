#!/usr/bin/env python3
import time
import datetime
from opcua import Client
from pymodbus.client.sync import ModbusTcpClient as ModbusClient

import logging
FORMAT = ('%(asctime)-15s %(threadName)-15s'
          ' %(levelname)-8s %(module)-15s:%(lineno)-8s %(message)s')
logging.basicConfig(format=FORMAT)
log = logging.getLogger()
log.setLevel(logging.DEBUG)


remoteIO1url = "opc.tcp://admin:admin@192.168.0.51:4840"
remoteIO2url = "opc.tcp://admin:admin@192.168.0.52:4840"
plcip = "192.168.0.30"

remoteIO1= Client(remoteIO1url)
remoteIO2= Client(remoteIO2url)

remoteIO1.connect()
remoteIO2.connect()
logging.debug("Client Connected")

orders = 2

# Define the process values
IO1input1value = False # Punching machine
IO1input2value = False
IO1output1value = False
IO1output2value = False
IO2input1value = False # Conveyor belt
IO2input2value = False
IO2output1value = False
IO2output2value = False

# Define the OPC-UA nodes
IO1input1  = remoteIO1.get_node("ns=1;s=Read Input 1.")
IO1input2  = remoteIO1.get_node("ns=1;s=Read Input 2.")
IO1output1 = remoteIO1.get_node("ns=1;s=Control Output 1.")
IO1output2 = remoteIO1.get_node("ns=1;s=Control Output 2.")
IO2input1  = remoteIO2.get_node("ns=1;s=Read Input 1.")
IO2input2  = remoteIO2.get_node("ns=1;s=Read Input 2.")
IO2output1 = remoteIO2.get_node("ns=1;s=Control Output 1.")
IO2output2 = remoteIO2.get_node("ns=1;s=Control Output 2.")

client = ModbusClient(plcip, port=502)
client.connect()

# Set OPC-UA mode on the PLC
rq = client.write_register(1032, 1, unit=1)

while True:
    # Generate Process image
    start_time = datetime.datetime.now()
    IO1input1value = IO1input1.get_value()
    IO1input2value = IO1input2.get_value()
    IO1output1value = IO1output1.get_value()
    IO1output2value = IO1output2.get_value()
    IO2input1value = IO2input1.get_value()
    IO2input2value = IO2input2.get_value()
    IO2output1value = IO2output1.get_value()
    IO2output2value = IO2output2.get_value()



    logging.debug("++++++++++++++++++++++++++++++++++++++")
    logging.debug("IO1input1:  " + str(IO1input1value))
    logging.debug("IO1input2:  " + str(IO1input2value))
    logging.debug("IO1output1: " + str(IO1output1value))
    logging.debug("IO1output2: " + str(IO1output2value))
    logging.debug("IO2input1:  " + str(IO2input1value))
    logging.debug("IO2input2:  " + str(IO2input2value))
    logging.debug("IO2output1: " + str(IO2output1value))
    logging.debug("IO2output2: " + str(IO2output2value))


    rq = client.write_register(1028, int(IO2input1value), unit=1)
    rq = client.write_register(1029, int(IO2input2value), unit=1)
    rq = client.write_register(1031, int(IO1input1value), unit=1)
    rq = client.write_register(1030, int(IO1input2value), unit=1)

    #rr = client.read_holding_registers(x, 1, unit=1)
    #print(str(rr.registers[0]))

    # Write back Process image
    IO1output1.set_value(bool(client.read_holding_registers(1024, 1, unit=1).registers[0]))
    IO1output2.set_value(bool(client.read_holding_registers(1025, 1, unit=1).registers[0]))
    IO2output1.set_value(bool(client.read_holding_registers(1026, 1, unit=1).registers[0]))
    IO2output2.set_value(bool(client.read_holding_registers(1027, 1, unit=1).registers[0]))

    end_time = datetime.datetime.now()

    time_diff = (end_time - start_time)
    execution_time = time_diff.total_seconds() * 1000

    logging.debug("Current cycle time is " + str(execution_time) + " ms")
