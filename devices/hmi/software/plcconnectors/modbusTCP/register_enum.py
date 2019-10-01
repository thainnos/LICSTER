"""
Helper classes.
"""
from enum import IntEnum


class ModbusTCPRegisters(IntEnum):
    """
    A simple enumeration of the modbus addresses used by the PLC with modbus/TCP.
    """
    PLCInputs = 800,
    ControlMotorUp = 1024,
    ControlMotorDown = 1025,
    ControlMotorLeft = 1026,
    ControlMotorRight = 1027,
    HmiApplicationState = 1034,
    PlcApplicationState = 1038,
    ProcessState = 1035,
    Orders = 1036,
    Reset = 1037

