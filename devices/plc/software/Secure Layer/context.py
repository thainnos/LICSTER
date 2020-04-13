from cryptography.hazmat.backends import default_backend
from cryptography.hazmat.primitives.asymmetric.ec import SECP256R1
from queue import Queue

class Context:
    def __init__(self):
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
