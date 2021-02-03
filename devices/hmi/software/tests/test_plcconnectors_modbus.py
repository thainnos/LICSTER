from plcconnectors.modbusTCP.register_enum import ModbusTCPRegisters
from plcconnectors.modbusTCP.connector import ModbusTCPPlcConnector
from unittest.mock import patch


class TestRegisterEnum:
    """
    Tests the helper class ModbusTCPRegisters in register_enum.py
    """
    def test_ModbusTCPRegisters(self):
        """
        Tests if the Values of the Helper class
        ModbusTCPRegisters remained unchanged.
        """
        assert int(ModbusTCPRegisters.PLCInputs) == 800
        assert int(ModbusTCPRegisters.ControlMotorUp) == 1024
        assert int(ModbusTCPRegisters.ControlMotorDown) == 1025
        assert int(ModbusTCPRegisters.ControlMotorLeft) == 1026
        assert int(ModbusTCPRegisters.ControlMotorRight) == 1027
        assert int(ModbusTCPRegisters.HmiApplicationState) == 1034
        assert int(ModbusTCPRegisters.PlcApplicationState) == 1038
        assert int(ModbusTCPRegisters.ProcessState) == 1035
        assert int(ModbusTCPRegisters.Orders) == 1036
        assert int(ModbusTCPRegisters.Reset) == 1037


class TestModbusTCPPlcConnector:
    """
    Tests the class ModbusTCPPlcConnector in connector.py
    """
    @patch("pymodbus.client.sync.ModbusTcpClient.connect")
    def test_ModbusTCPPlcConnector_init_correct(self, mock_connect):
        mock_motor_controls = {
            "control-motor-up": int(ModbusTCPRegisters.ControlMotorUp),
            "control-motor-down": int(ModbusTCPRegisters.ControlMotorDown),
            "control-motor-left": int(ModbusTCPRegisters.ControlMotorLeft),
            "control-motor-right": int(ModbusTCPRegisters.ControlMotorRight)
        }
        mock_motor_names = ["motorUp", "motorDown", "motorLeft", "motorRight"]
        mock_sensor_names = [
            "topSensor",
            "bottomSensor",
            "leftSensor",
            "rightSensor"]

        mock_connect.return_value = True
        modbusTCPPlcConnector = ModbusTCPPlcConnector("172.0.0.1")

        assert modbusTCPPlcConnector.modbus_client.connect() is True
        assert modbusTCPPlcConnector.motor_controls == mock_motor_controls
        assert modbusTCPPlcConnector.motor_names == mock_motor_names
        assert modbusTCPPlcConnector.sensor_names == mock_sensor_names

    @patch("pymodbus.client.sync.ModbusTcpClient.is_socket_open")
    def test_ModbusTCPPlcConnector_is_connected(self, mock_socket_open):
        mock_socket_open.return_value = True
        modbusTCPPlcConnector = ModbusTCPPlcConnector("172.0.0.1")
        assert modbusTCPPlcConnector.is_connected() is True

    @patch("pymodbus.client.sync.ModbusTcpClient.read_discrete_inputs")
    @patch("pymodbus.client.sync.ModbusTcpClient.read_coils")
    @patch("pymodbus.client.sync.ModbusTcpClient.is_socket_open")
    def test_ModbusTCPPlcConnector_get_values(self, mock_socket_open,
                                              mock_read_coils,
                                              mock_read_descrete_inputs):
        # Condition for these Tests:
        # The Plc IS reachable (mocked)

        # TODO Find out what the Response object should be
        correct_sensors_and_motors = {'topSensor': '0', 'bottomSensor': '0',
                                      'leftSensor': '0', 'rightSensor': '0',
                                      'motorUp': 'true', 'motorDown': 'true',
                                      'motorLeft': 'true',
                                      'motorRight': 'true'}

        class Response:
            def __init__(self, array):
                self.bits = array

        mock_socket_open.return_value = True
        mock_read_coils.return_value = Response([0, 0, 0, 0])
        mock_read_descrete_inputs.return_value = Response([0, 0, 0, 0])

        modbusTCPPlcConnector = ModbusTCPPlcConnector("172.0.0.1")
        sensors_and_motors = modbusTCPPlcConnector.get_values()
        assert sensors_and_motors == correct_sensors_and_motors

        # Condition for these Tests:
        # The Plc is NOT reachable (mocked)
        del modbusTCPPlcConnector
        correct_sensors_and_motors = {'topSensor': 'true', 'bottomSensor':
                                      'true', 'leftSensor': 'true',
                                      'rightSensor': 'true', 'motorUp': 'true',
                                      'motorDown': 'true', 'motorLeft': 'true',
                                      'motorRight': 'true'}

        mock_socket_open.return_value = False
        mock_read_coils.return_value = Response(b'0000')
        mock_read_descrete_inputs.return_value = Response(b'0000')

        modbusTCPPlcConnector = ModbusTCPPlcConnector("172.0.0.1")
        sensors_and_motors = modbusTCPPlcConnector.get_values()
        assert sensors_and_motors == correct_sensors_and_motors

    @patch('pymodbus.client.sync.ModbusTcpClient.read_coils',
           **{'return_value.raiseError.side_effect': BaseException()})
    @patch("pymodbus.client.sync.ModbusTcpClient.is_socket_open")
    def test_ModbusTCPPlcConnector_get_values_raisedError(self,
                                                          mock_socket_open,
                                                          mock_read_coils):
        """
        Test Handling of Base Exception with connected PLC
        """
        correct_sensors_and_motors = {'topSensor': 'true', 'bottomSensor':
                                      'true', 'leftSensor': 'true',
                                      'rightSensor': 'true', 'motorUp': 'true',
                                      'motorDown': 'true', 'motorLeft': 'true',
                                      'motorRight': 'true'}

        mock_socket_open.return_value = True
        mock_read_coils.return_value.raiseError.side_effect = BaseException(
            'Test')

        modbusTCPPlcConnector = ModbusTCPPlcConnector("172.0.0.1")
        sensors_and_motors = modbusTCPPlcConnector.get_values()
        assert sensors_and_motors == correct_sensors_and_motors

    @patch("pymodbus.client.sync.ModbusTcpClient.write_register")
    @patch("pymodbus.client.sync.ModbusTcpClient.is_socket_open")
    def test_ModbusTCPPlcConnector_set_order(self, mock_socket_open,
                                             mock_write_register):
        mock_socket_open.return_value = True
        modbusTCPPlcConnector = ModbusTCPPlcConnector("172.0.0.1")
        modbusTCPPlcConnector.set_order(5)
        mock_write_register.assert_called_once()

    @patch("pymodbus.client.sync.ModbusTcpClient.write_register")
    @patch("pymodbus.client.sync.ModbusTcpClient.is_socket_open")
    def test_ModbusTCPPlcConnector_set_app_state(self, mock_socket_open,
                                                 mock_write_register):
        # Connected to the PLC
        mock_socket_open.return_value = True
        mock_write_register.return_value = b"01"

        class StopState():
            def __init__(self):
                self.modbus_value = 2
        state = StopState()

        modbusTCPPlcConnector = ModbusTCPPlcConnector("172.0.0.1")
        return_value = modbusTCPPlcConnector.set_application_state(state)
        assert return_value == b"01"

        # Not connected to the PLC
        mock_socket_open.return_value = False
        modbusTCPPlcConnector = ModbusTCPPlcConnector("172.0.0.1")
        return_value = modbusTCPPlcConnector.set_application_state(state)
        assert return_value == 3

    @patch("pymodbus.client.sync.ModbusTcpClient.write_register")
    @patch("pymodbus.client.sync.ModbusTcpClient.is_socket_open")
    def test_ModbusTCPPlcConnector_set_motor(self, mock_socket_open,
                                             mock_write_register):
        # Connected to the PLC
        mock_socket_open.return_value = True
        mock_write_register.return_value = b"01"
        modbusTCPPlcConnector = ModbusTCPPlcConnector("127.0.0.1")
        return_values = []
        motors = ["control-motor-up", "control-motor-down",
                  "control-motor-left", "control-motor-right"]

        for motor in motors:
            result = modbusTCPPlcConnector.set_motor(motor, "0")
            return_values.append(result)

        assert return_values == [b"01", b"01", b"01", b"01"]

        # Not connected to the PLC
        mock_socket_open.return_value = False
        return_values = []
        motors = ["control-motor-up", "control-motor-down",
                  "control-motor-left", "control-motor-right"]

        for motor in motors:
            result = modbusTCPPlcConnector.set_motor(motor, "0")
            return_values.append(result)

        assert return_values == [3, 3, 3, 3]

    @patch("pymodbus.client.sync.ModbusTcpClient.write_register")
    @patch("pymodbus.client.sync.ModbusTcpClient.is_socket_open")
    def test_ModbusTCPPlcConnector_set_reset(self, mock_socket_open,
                                             mock_write_register):
        # Connected to the PLC
        mock_socket_open.return_value = True
        mock_write_register.return_value = b"01"
        modbusTCPPlcConnector = ModbusTCPPlcConnector("127.0.0.1")
        result = modbusTCPPlcConnector.set_reset()
        assert result == b"01"

        # Not connected to the PLC
        mock_socket_open.return_value = False
        result = modbusTCPPlcConnector.set_reset()
        assert result == 3

    @patch("pymodbus.client.sync.ModbusTcpClient.read_holding_registers")
    @patch("pymodbus.client.sync.ModbusTcpClient.is_socket_open")
    def test_ModbusTCPPlcConnector_get_orders(self, mock_socket_open,
                                              mock_read_register):
        # Connected to the PLC
        mock_socket_open.return_value = True

        class Response:
            def __init__(self, array):
                self.registers = array
        mock_read_register.return_value = Response([b"01", b"00"])
        modbusTCPPlcConnector = ModbusTCPPlcConnector("127.0.0.1")
        result = modbusTCPPlcConnector.get_orders()
        assert result == b"01"

        # Not connected to the PLC
        mock_socket_open.return_value = False
        result = modbusTCPPlcConnector.get_orders()
        assert result == 3

    @patch("pymodbus.client.sync.ModbusTcpClient.read_holding_registers")
    @patch("pymodbus.client.sync.ModbusTcpClient.is_socket_open")
    def test_ModbusTCPPlcConnector_get_proc_states(self, mock_socket_open,
                                                   mock_read_register):
        # Connected to the PLC
        mock_socket_open.return_value = True

        class Response:
            def __init__(self, array):
                self.registers = array
        mock_read_register.return_value = Response([b"01", b"00"])
        modbusTCPPlcConnector = ModbusTCPPlcConnector("127.0.0.1")
        result = modbusTCPPlcConnector.get_process_state()
        assert result == b"01"

        # Not connected to the PLC
        mock_socket_open.return_value = False
        result = modbusTCPPlcConnector.get_process_state()
        assert result == 0

    @patch("pymodbus.client.sync.ModbusTcpClient.read_holding_registers")
    @patch("pymodbus.client.sync.ModbusTcpClient.is_socket_open")
    def test_ModbusTCPPlcConnector_get_app_state(self, mock_socket_open,
                                                 mock_read_register):
        # Connected to the PLC
        mock_socket_open.return_value = True

        class Response:
            def __init__(self, array):
                self.registers = array
        mock_read_register.return_value = Response([b"01", b"00"])
        modbusTCPPlcConnector = ModbusTCPPlcConnector("127.0.0.1")
        result = modbusTCPPlcConnector.get_application_state()
        assert result == b"01"

        # Not connected to the PLC
        mock_socket_open.return_value = False
        result = modbusTCPPlcConnector.get_application_state()
        assert result == 3
