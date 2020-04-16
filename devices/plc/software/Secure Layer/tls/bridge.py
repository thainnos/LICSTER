import socket
import ssl
from queue import Empty, Queue
from threading import Thread

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

        self.start()
        
        self._error_q = Queue()


    def run(self):
        self._init_connections()
        while True:
            message = self.ctx.q_manage_in.get()
            if message == 'END':
                return
            elif message == '_Error':
                self.ctx.q_manage_out.put(('Error', self))
                return

    def _start_connection_handlers(self):
        # TODO: still awaits one connection after error?
        # TODO: this is still wrong, need send_plc_message!
        def handle_plc_incoming():
            while True:
                try:
                    msg = self.conn_plc.recv(MAX_PACKET_SIZE)
                    self.send_message(self._send_message(msg, 'remoteIO'))
                except:
                    self.ctx.q_manage_in.put('_Error')
                    self._error_q.put('Error')
                    return
                try:
                    msg = self._error_q.get_nowait()
                    if msg == 'Error':
                        return
                except Empty:
                    continue
        def handle_io_incoming():
            while True:
                try:
                    msg = self.ssock_io.recv(MAX_PACKET_SIZE)
                    self.send_message(self._send_message(msg, 'plc'))
                except:
                    self.ctx.q_manage_in.put('_Error')
                    self._error_q.put('Error')
                    return
                try:
                    msg = self._error_q.get_nowait()
                    if msg == 'Error':
                        return
                except Empty:
                    continue
        
        Thread(target=handle_plc_incoming).start()
        Thread(target=handle_io_incoming).start()

    def _init_connections(self):
        self._init_io_connection()
        self._init_plc_connection()

    def _init_io_connection(self):
        self._init_ssl_context()
        self.sock_io = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        # TODO: maybe change server_hostname
        self.ssock_io = self.ssl_context.wrap_socket(
            self.sock_io, server_side=False, server_hostname=self.ctx.io_name)

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
        if dest == 'remoteIO':
            self.ssock_io.send(msg)
        elif dest == 'plc':
            self.conn_plc.send(msg)
        else:
            raise AttributeError('invalid destination')
