#!/usr/bin/python3

# python -m pip install pymodbus
# python3 -m pip install pymodbus

import coloredlogs, logging
from pymodbus.client.sync import ModbusTcpClient
import time
import sys

# Starting logger
coloredlogs.install()
logging.info("Starting logger")

try:
    while True:
        try:
            client = ModbusTcpClient('192.168.0.52')
            client.write_coil(1, False)
            client.close() 

        except KeyboardInterrupt:
            logging.error('interrupted!')
            sys.exit();
        except Exception as e:
            logging.error('Exception: ' + str(e))
            client.close();

except KeyboardInterrupt:
        logging.error('interrupted!')
        sys.exit();
