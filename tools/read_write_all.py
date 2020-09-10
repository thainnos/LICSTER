#!/usr/bin/python3

# python -m pip install pymodbus
# python3 -m pip install pymodbus

import coloredlogs
import logging
from pymodbus.client.sync import ModbusTcpClient
import time
import sys

# Starting logger
coloredlogs.install()
logging.info("Starting logger")

try:
    while True:
        try:
            for output in range(1, 4):
                client1 = ModbusTcpClient('192.168.0.30')
                time.sleep(0.5)
                # logging.info("Set value " + str(output) + " to True!")
                # client1.write_coil(output, True)
                result = client1.read_coils(0, output)
                logging.info("Value True: " + str(result.bits[0]))
                client1.close()

        except KeyboardInterrupt:
            logging.error('interrupted!')
            sys.exit()
        except Exception as e:
            logging.error('Exception: ' + str(e))
            client1.close()

except KeyboardInterrupt:
    logging.error('interrupted!')
    sys.exit()
