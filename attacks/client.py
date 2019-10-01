from pymodbus.client.sync import ModbusTcpClient
import time

client = ModbusTcpClient("192.168.0.30")

def setOrder(order=0):
    client.write_register(1036, order)

def manual():
    client.write_register(1034, 1)


def automatic():
    client.write_register(1034, 0)


def stop():
    client.write_register(1034, 2)


def moveUp():
    manual()
    client.write_register(1024, 1)
    time.sleep(0.5)
    client.write_register(1024, 0)


def moveDown():
    manual()
    client.write_register(1025, 1)
    time.sleep(0.5)
    client.write_register(1025, 0)


def moveLeft():
    manual()
    client.write_register(1026, 1)
    time.sleep(0.5)
    client.write_register(1026, 0)


def moveRight():
    manual()
    client.write_register(1027, 1)
    time.sleep(0.5)
    client.write_register(1027, 0)
