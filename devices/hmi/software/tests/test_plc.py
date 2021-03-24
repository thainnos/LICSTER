from plcconnectors.plc import Plc
from unittest.mock import patch
from plcconnectors.modbusTCP.connector import ModbusTCPPlcConnector
from plcconnectors.modbusTCP.connector import ModbusTcpClient


class TestPlc:
    def test_plc_init(self):
        plc = Plc(ModbusTCPPlcConnector, "127.0.0.1", 10)
        assert isinstance(plc.plc_connector, ModbusTCPPlcConnector)
        assert isinstance(plc.plc_connector.modbus_client, ModbusTcpClient)

    @patch("plcconnectors.modbusTCP.connector.ModbusTCPPlcConnector."
           "is_connected")
    def test_plc_is_connected(self, mock_is_connected):
        mock_is_connected.return_value = True
        plc = Plc(ModbusTCPPlcConnector, "127.0.0.1", 10)
        assert plc.is_connected() is True

        mock_is_connected.return_value = False
        assert plc.is_connected() is False

    def test_set_state(self):
        pass

    def test_set_order(self):
        pass

    def test_set_motor(self):
        pass

    def test_set_reset(self):
        pass

    def test_get_orders(self):
        pass

    def test_get_process_values(self):
        pass

    def test_get_process_state(self):
        pass

    def test_get_aaplication_state(self):
        pass
