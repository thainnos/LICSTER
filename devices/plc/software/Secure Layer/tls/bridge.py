import socket
import ssl
import time
from queue import Empty, Queue
from threading import Lock, Thread

from context import Context

MAX_PACKET_SIZE = 1024



class Bridge(Thread):
    def __init__(self, context: Context):
        super(Bridge, self).__init__()

        self.ctx = context
        self.name = 'Bridge-' + self.ctx.name
        self.sock_plc = None
        self.conn_plc = None
        self.sock_io = None
        self.ssock_io = None
        self.ssl_context = None

        self.conn_start = None

        self.start()
        
        self._error_q = Queue()


    def run(self):
        self._init_connections()
        self._start_connection_handler()
        while True:
            message = self.ctx.q_manage_in.get()
            if message == 'END':
                self._close_connections()
                return
            elif message == '_Error':
                self.ctx.q_manage_out.put(('Error', self))
                self._close_connections()
                return

    def _close_connections(self):
        self.sock_plc.close()
        self.sock_io.close()

    @staticmethod
    def _check_msg(msg: bytes):
        if msg == b'':
            raise Exception('connection closed')
        return msg

    def _start_connection_handler(self):
        # TODO: run this as process so you can kill externally?
        def handle_plc_incoming():
            while True:
                try:
                    # recv from plc and send to io
                    msg = self._check_msg(self.conn_plc.recv(MAX_PACKET_SIZE))
                    # print('PLC:', msg)
                    self._send_message(msg, 'remoteIO')
                    
                    # recv from io and send to plc
                    msg = self._check_msg(self.ssock_io.recv(MAX_PACKET_SIZE))
                    # print('IO:', msg)
                    self._send_message(msg, 'plc')
                    
                except Exception as e:
                    print(e)
                    self.ctx.q_manage_in.put('_Error')
                    return
        
        Thread(target=handle_plc_incoming).start()

    def _init_connections(self):
        self._init_io_connection()
        self._init_plc_connection()

    def _init_io_connection(self):
        self._init_ssl_context()
        self.sock_io = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        if self.ctx.secure:
            # TODO: maybe change server_hostname
            # TODO: handle bad handshake
            self.ssock_io = self.ssl_context.wrap_socket(
                self.sock_io, server_side=False, server_hostname=self.ctx.io_name)
            self.ssock_io.connect((self.ctx.host_io, self.ctx.port_io))
            print('Connected to remotIO in ssl')
            print(self.ssock_io.version())
        else:
            self.ssock_io = self.sock_io
            self.ssock_io.connect((self.ctx.host_io, self.ctx.port_io))
            print('Connected to remotIO')

    def _init_plc_connection(self):
        # TODO: verify this
        socket_type = getattr(socket, 'AF_UNIX' if hasattr(socket, 'AF_UNIX') else 'AF_INET')
        self.sock_plc = socket.socket(socket_type, socket.SOCK_STREAM)
        self.sock_plc.bind(('', self.ctx.port_plc))
        self.sock_plc.listen()
        self.conn_plc, _addr = self.sock_plc.accept()
        print('plc connected')
        print(f'connected {_addr[0]}:{_addr[1]} to localhost:{self.ctx.port_plc}')

    def _init_ssl_context(self):
        self.ssl_context = ssl.create_default_context(
            ssl.Purpose.SERVER_AUTH, cafile=self.ctx.io_cert)
        self.ssl_context.load_cert_chain(
            certfile=self.ctx.plc_cert, keyfile=self.ctx.plc_key)

    def _send_message(self, msg: bytes, dest: str):
        dest = dest.lower()
        if dest == 'remoteio':
            self.ssock_io.send(msg)
        elif dest == 'plc':
            self.conn_plc.send(msg)
        else:
            raise AttributeError('invalid destination')
