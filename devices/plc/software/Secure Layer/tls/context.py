from configparser import ConfigParser
from queue import Queue


class Context:
    def __init__(self):
        self.name = 'Unnamed'

        # self.io_hostname = 'Invalid'

        self.io_cert = None
        self.plc_cert = None
        self.plc_key = None
        self.io_name = None

        self.port_plc = None

        self.host_io = None
        self.port_io = None

        self.q_manage_in = Queue()
        self.q_manage_out = Queue()

    # TODO: error handling, for misconfigured or missing config
    @staticmethod
    def load_from_config(filename: str):
        config = ConfigParser()
        config.read(filename)
        contexts = []
        
        plc_cert = config.get('general', 'plc_cert')
        plc_key = config.get('general', 'plc_key')

        for section in config.sections():
            if 'bridge' != section[:6]:
                continue
            context = Context()
            context.plc_cert = plc_cert
            context.plc_key = plc_key
            context.name = section
            context.port_plc = config.getint(section, 'local_port')
            context.port_io = config.getint(section, 'remote_port')
            context.host_io = config.get(section, 'remote_address')
            context.io_cert = config.get(section, 'io_cert')
            context.io_name = config.get(section, 'io_name')
            contexts.append(context)
        
        return contexts
