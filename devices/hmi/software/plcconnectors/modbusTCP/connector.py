"""
The connector to the plc for modbus/TCP.
"""
from .register_enum import ModbusTCPRegisters
from pymodbus.client.sync import ModbusTcpClient
import sys


class ModbusTCPPlcConnector:
    """
    Instantiates the connection to a PLC via modbus/TCP.
    """

    def __init__(self, plc_ip_address):
        """
        :param plc_ip_address: The IP address of the PLC.
        """
        self.modbus_client = ModbusTcpClient(plc_ip_address)
        self.modbus_client.connect()
        if not self.modbus_client.is_socket_open():
            print("Cannot connect plc connector to {0}".format(plc_ip_address))
            raise ValueError('No connection to OpenPLC.')

        # Map of motor controls to modbus/TCP bits.
        self.motor_controls = {
            "control-motor-up": int(ModbusTCPRegisters.ControlMotorUp),
            "control-motor-down": int(ModbusTCPRegisters.ControlMotorDown),
            "control-motor-left": int(ModbusTCPRegisters.ControlMotorLeft),
            "control-motor-right": int(ModbusTCPRegisters.ControlMotorRight)
        }
        # The proper names for the motor controls.
        self.motor_names = ["motorUp", "motorDown", "motorLeft", "motorRight"]

        # The proper names for the sensors.
        self.sensor_names = ["topSensor", "bottomSensor", "leftSensor", "rightSensor"]

    def get_values(self):
        """
        This function queries the state of the sensors and motor controls.
        :return: A dictionary of the state of the motor controls and the state of the sensors.
        """

        # Read the four bits of motor state from the PLC over modbus/TCP.
        motor_values = self.modbus_client.read_coils(int(ModbusTCPRegisters.PLCInputs), 4).bits[:4]

        # Read the four bits of sensors state from the PLC over modbus/TCP.
        sensor_values = self.modbus_client.read_discrete_inputs(int(ModbusTCPRegisters.PLCInputs)).bits[:4]

        # Separate the limit switches of the punching arm from the light switches
        limit_switches = sensor_values[:2]

        # Separate the light switches from the limit switches of the punching arm.
        light_sensors = sensor_values[2:4]

        # The values of the motor manual values must be inverted to fit our logic.
        # The values are converted to strings.
        corrected_motor_values = [str(not value).lower() for value in motor_values]

        # The values are converted to strings.
        light_sensor_values = [str(value).lower() for value in light_sensors]

        # The values are converted to strings.
        limit_switch_values = [str(value).lower() for value in limit_switches]

        # Join the sensor values again.
        joined_sensor_values = limit_switch_values + light_sensor_values

        # Join the proper sensor names as keys with the corresponding sensor values into a dictionary.
        sensors = dict(zip(self.sensor_names, joined_sensor_values))

        # Join the proper motor manual names as keys with the corresponding motor manual values into a dictionary.
        motors = dict(zip(self.motor_names, corrected_motor_values))

        # Return the dictionary that contains the current motor manual values as well as sensor values.
        return {**sensors, **motors}

    def set_order(self, count):
        """
        Set an positive numeric value as amount of times the process is to be executed.
        :param count: Positive integer.
        :return: The PLC connectors response, so it can be use if of interest.
        """
        self.modbus_client.write_register(int(ModbusTCPRegisters.Orders), count)

    def set_application_state(self, state):
        """
        Set the current application state.
        :param state:
        :return: The PLC connectors response, so it can be use if of interest.
        """
        return self.modbus_client.write_register(int(ModbusTCPRegisters.HmiApplicationState), state.modbus_value)

    def set_motor(self, motor, motor_state):
        """
        Turn a motor of the process manually on or off.
        :param motor: Which motor is to be set.
        :param motor_state: On or Off.
        :return: The PLC connectors response, so it can be use if of interest.
        """
        register = self.motor_controls[motor]
        return self.modbus_client.write_register(register, int(motor_state))

    def set_reset(self):
        """
        Initiate the reset signal to get the PLC out of the emergency stop state.
        :return: The PLC connectors response, so it can be use if of interest.
        """
        return self.modbus_client.write_register(int(ModbusTCPRegisters.Reset), 1)

    def get_orders(self):
        """
        Retrieve the amount of currently placed orders.
        :return: The order count.
        """
        return self.modbus_client.read_holding_registers(int(ModbusTCPRegisters.Orders), 1).registers[0]

    def get_process_state(self):
        """
        Query the process state.
        :return: A dictionary containing the current process state.
        """
        return self.modbus_client.read_holding_registers(int(ModbusTCPRegisters.ProcessState), 2).registers[0]

    def get_application_state(self):
        """
        Query the application state.
        :return: A dictionary containing the current application state.
        """
        return self.modbus_client.read_holding_registers(int(ModbusTCPRegisters.PlcApplicationState), 1).registers[0]
