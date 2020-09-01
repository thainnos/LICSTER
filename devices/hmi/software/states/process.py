"""
The possible states that the process can be in.
"""
from states.singleton import Singleton


class PendingState(metaclass=Singleton):
    """
    The state in between the production state.
    """

    def __init__(self):
        self.name = "pending"
        self.text = "Pending"
        self.modbus_value = 0
        self.classes = ["btn btn-success mr-auto"]


class TowardsPunchingMachineState(metaclass=Singleton):
    """
    The process step where the conveyor belt moves the goods towards the
    punching machine.
    """

    def __init__(self):
        self.name = "to-punch"
        self.text = "Move Right"
        self.modbus_value = 1
        self.classes = ["btn btn-warning mr-auto"]


class PunchingMachineDownState(metaclass=Singleton):
    """
    The process step where the punching machine moves down to punch on the
    goods.
    """

    def __init__(self):
        self.name = "punch-down"
        self.text = "Move Down"
        self.modbus_value = 2
        self.classes = ["btn btn-warning mr-auto"]


class PunchingMachineUpState(metaclass=Singleton):
    """
    The process step where the punching machine moves up.
    """

    def __init__(self):
        self.name = "punch-up"
        self.text = "Move Up"
        self.modbus_value = 3
        self.classes = ["btn btn-warning mr-auto"]


class FromPunchingMachineState(metaclass=Singleton):
    """
    The process step where the conveyor belt moves the goods towards the
    punching machine.
    """

    def __init__(self):
        self.name = "from-punch"
        self.text = "Move Left"
        self.modbus_value = 4
        self.classes = ["btn btn-warning mr-auto"]


class ProcessStateMap(metaclass=Singleton):
    """
    A simple helper class to navigate the states.
    """

    def __init__(self):
        self.states = {
            PendingState().name: PendingState(),
            PendingState().modbus_value: PendingState(),
            TowardsPunchingMachineState().name: TowardsPunchingMachineState(),
            TowardsPunchingMachineState().modbus_value:
                TowardsPunchingMachineState(),
            PunchingMachineDownState().name: PunchingMachineDownState(),
            PunchingMachineDownState().modbus_value:
                PunchingMachineDownState(),
            PunchingMachineUpState().name: PunchingMachineUpState(),
            PunchingMachineUpState().modbus_value: PunchingMachineUpState(),
            FromPunchingMachineState().name: FromPunchingMachineState(),
            FromPunchingMachineState().modbus_value: FromPunchingMachineState()
        }
