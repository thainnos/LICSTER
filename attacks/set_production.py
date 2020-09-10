#!/usr/bin/python3

# python -m pip install pymodbus
# python3 -m pip install pymodbus

import coloredlogs
import logging
from pymodbus.client.sync import ModbusTcpClient
import sys

# Starting logger
coloredlogs.install()
logging.info("Starting logger")

try:
    client = ModbusTcpClient('192.168.0.30')
    while True:
        try:
            client.write_register(1024, 2, unit=1)
            response = client.read_holding_registers(1024,
                                                     1, unit=1).registers[0]
            # logging.info("Register: " + str(response.registers))

        except KeyboardInterrupt:
            client.close()
            logging.error('interrupted!')
            sys.exit()

        except Exception as e:
            logging.error('Exception: ' + str(e))
            client.close()

except KeyboardInterrupt:
    client.close()
    logging.error('interrupted!')
    sys.exit()
