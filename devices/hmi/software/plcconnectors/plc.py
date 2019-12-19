"""
The programmable logic controller (PLC) is the interface to the actual PLC. Its main purpose is to instantiate the
plc_connector_class and forward its own function invocations to it as well as return its results. It serves as a
generic layer to the web application.
"""
from states.application import ApplicationStateMap
from states.process import ProcessStateMap


class Plc:
    """
    Representation of the PLC. Its state represents the state of actual PLC.
    """

    def __init__(self, plc_connector_class, plc_ip_address, timeout=0):
        """
        :param plc_connector_class: A concrete PLC connector.
        :param plc_ip_address: The IP address of the PLC.
        """
        self.plc_connector = plc_connector_class(plc_ip_address, timeout=timeout)

    def is_connected(self):
        """
        Is the plc connected with the actual plc?
        :return: True if the underlying plc connector claims that it is connected
        """
        return self.plc_connector.is_connected()

    def set_state(self, state):
        """
        Set the current application state.
        :param state:
        :return: The PLC connectors response, so it can be use if of interest.
        """
        application_state = ApplicationStateMap().states[state]
        return self.plc_connector.set_application_state(application_state)

    def set_order(self, count):
        """
        Set an positive numeric value as amount of times the process is to be executed.
        :param count: Positive integer.
        :return: The PLC connectors response, so it can be use if of interest.
        """
        return self.plc_connector.set_order(count)

    def set_motor(self, motor, motor_state):
        """
        Turn a motor of the process manually on or off.
        :param motor: Which motor is to be set.
        :param motor_state: On or Off.
        :return: The PLC connectors response, so it can be use if of interest.
        """
        return self.plc_connector.set_motor(motor, motor_state)

    def set_reset(self):
        """
        Initiate the reset signal to get the PLC out of the emergency stop state.
        :return: The PLC connectors response, so it can be use if of interest.
        """
        return self.plc_connector.set_reset()

    def get_orders(self):
        """
        Retrieve the amount of currently placed orders.
        :return: The order count.
        """
        return self.plc_connector.get_orders()

    def get_process_values(self):
        """
        Retrieve the sate of all sensors and motors in a single dictionary.
        :return: A dictionary containing all sensor and motor states.
        """
        return self.plc_connector.get_values()

    def get_process_state(self):
        """
        Query the process state.
        :return: A dictionary containing the current process state.
        """
        state_value = self.plc_connector.get_process_state()
        state = ProcessStateMap().states[state_value]
        return state.__dict__

    def get_application_state(self):
        """
        Query the application state.
        :return: A dictionary containing the current application state.
        """
        state_value = self.plc_connector.get_application_state()
        state = ApplicationStateMap().states[state_value]
        return state.__dict__
