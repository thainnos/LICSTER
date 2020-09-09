"""
The possible states that the application can be in.
"""
from states.singleton import Singleton


class AutomaticState(metaclass=Singleton):
    """
    Automatic is the default state. No manual control possible.
    """

    def __init__(self):
        self.modbus_value = 0
        self.text = "Automatic"
        self.name = "automatic"
        self.classes = ["btn btn-success mr-auto"]


class ManualState(metaclass=Singleton):
    """
    The manual state does not allow for automatic process execution,
    yet manual control.
    """

    def __init__(self):
        self.modbus_value = 1
        self.text = "Manual"
        self.name = "manual"
        self.classes = ["btn btn-warning mr-auto"]


class StopState(metaclass=Singleton):
    """
    The stop state is an emergency state which can only be reverted by the
    reset trigger.
    """

    def __init__(self):
        self.modbus_value = 2
        self.text = "STOPPED"
        self.name = "stop"
        self.classes = ["btn btn-danger mr-auto"]


class Disconnected(metaclass=Singleton):
    """
    The stop state is an emergency state which can only be reverted by the
    reset trigger.
    """

    def __init__(self):
        self.modbus_value = 3
        self.text = "DISCONNECTED"
        self.name = "disconnected"
        self.classes = ["btn btn-info mr-auto"]


class ApplicationStateMap(metaclass=Singleton):
    """
    A simple helper class to navigate the states.
    """

    def __init__(self):
        self.states = {
            AutomaticState().name: AutomaticState(),
            AutomaticState().modbus_value: AutomaticState(),
            ManualState().name: ManualState(),
            ManualState().modbus_value: ManualState(),
            StopState().name: StopState(),
            StopState().modbus_value: StopState(),
            Disconnected().name: Disconnected(),
            Disconnected().modbus_value: Disconnected(),
        }
