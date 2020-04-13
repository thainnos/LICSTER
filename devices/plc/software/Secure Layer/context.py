from configparser import ConfigParser
from queue import Queue

from cryptography.hazmat.backends import default_backend
from cryptography.hazmat.primitives.asymmetric.ec import SECP256R1


class Context:
    def __init__(self):
        self.name = 'Unnamed'
        self.backend = default_backend()
        self.curve = SECP256R1()

        self.socket_plc = None
        self.port_plc = None
        self.conn_plc = None

        self.socket_io = None
        self.host_io = None
        self.port_io = None
        self.cipher = None

        self.q_manage_in = Queue()
        self.q_manage_out = Queue()

        self.shared_key = None
    
    # TODO: error handling, for misconfigured or missing config
    @staticmethod
    def load_from_config(filename: str):
        config = ConfigParser()
        config.read(filename)
        contexts = []
        for section in config.sections():
            context = Context()
            context.name = section
            context.port_plc = config.getint(section, 'local_port')
            context.port_io = config.getint(section, 'remote_port')
            context.host_io = config.get(section, 'remote_address')
            contexts.append(context)
        return contexts
