#!/usr/bin/env python3
import time
import datetime
from opcua import Client

remoteIO1url = "opc.tcp://admin:admin@192.168.0.51:4840"
remoteIO2url = "opc.tcp://admin:admin@192.168.0.52:4840"

remoteIO1= Client(remoteIO1url)
remoteIO2= Client(remoteIO2url)

remoteIO1.connect()
remoteIO2.connect()
print("Client Connected")

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

# Define Automatic States
state = 1

while True:
    # Generate Process image
    start_time = datetime.datetime.now()
    IO1input1value = IO1input1.get_value()
    IO1input2value = IO1input2.get_value()
    IO1output1value = IO1output1.get_value()
    IO1output2value = IO1output2.get_value()
    IO2input1value = not IO2input1.get_value()
    IO2input2value = not IO2input2.get_value()
    IO2output1value = IO2output1.get_value()
    IO2output2value = IO2output2.get_value()



    print("++++++++++++++++++++++++++++++++++++++")
    print("IO1input1:  " + str(IO1input1value))
    print("IO1input2:  " + str(IO1input2value))
    print("IO1output1: " + str(IO1output1value))
    print("IO1output2: " + str(IO1output2value))
    print("IO2input1:  " + str(IO2input1value))
    print("IO2input2:  " + str(IO2input2value))
    print("IO2output1: " + str(IO2output1value))
    print("IO2output2: " + str(IO2output2value))

    print("Current state is " + str(state))

    error_count = 1000;

    if orders:
        if state == 1: # Waiting for orders to start execution"
            state = 2
        elif state == 2: # Waiting for orders to start execution"
            IO1output1value = False
            IO1output2value = False
            IO2output1value = False
            IO2output2value = False
            if IO2input1value is True:
                IO1output1value = False
                IO1output2value = False
                IO2output1value = False
                IO2output2value = False
                state = 3
        elif state == 3: # Moving the conveyor belt towards punching machine"
            IO1output1value = False
            IO1output2value = False
            IO2output1value = True
            IO2output2value = False
            if IO2input2value is True:
                IO1output1value = False
                IO1output2value = False
                IO2output1value = False
                IO2output2value = False
                state = 4
        elif state == 4: # "Moving the punching machine down to punch good"
            IO1output1value = False
            IO1output2value = True
            IO2output1value = False
            IO2output2value = False
            if IO1input2value is True:
                IO1output1value = False
                IO1output2value = False
                IO2output1value = False
                IO2output2value = False
                state = 5
        elif state == 5: # "Moving the punching machine up"
            IO1output1value = True
            IO1output2value = False
            IO2output1value = False
            IO2output2value = False
            if IO1input1value is True:
                IO1output1value = False
                IO1output2value = False
                IO2output1value = False
                IO2output2value = False
                state = 6
        elif state == 6: # "Moving the conveyor belt away from the punching machine"
            IO1output1value = False
            IO1output2value = False
            IO2output1value = False
            IO2output2value = True
            if IO2input1value is True:
                IO1output1value = False
                IO1output2value = False
                IO2output1value = False
                IO2output2value = False
                orders = orders - 1
                state = 1
        else: # "go into EMERGENCY STOP"
            printf("error")



    # Set the cycle time
    time.sleep(0.05)

    # Write back Process image
    IO1output1.set_value(IO1output1value)
    IO1output2.set_value(IO1output2value)
    IO2output1.set_value(IO2output1value)
    IO2output2.set_value(IO2output2value)

    end_time = datetime.datetime.now()

    time_diff = (end_time - start_time)
    execution_time = time_diff.total_seconds() * 1000

    print("Current cycle time is " + str(execution_time) + " ms")
