"""
This module contains the Bridge class.
"""
import socket
import ssl
from multiprocessing import Process
from queue import Queue
from struct import unpack
from threading import Thread
from typing import Tuple

from config import Config

MAX_PACKET_SIZE = 1024
HEADER_SIZE = 6


class Bridge(Process):
    """
    A single bridge that manages the connection between 2 devices.
    """
    def __init__(self, context: Config):
        super(Bridge, self).__init__()

        self.cfg = context
        self.name = 'Bridge-' + self.cfg.name
        self.sock_plc = None
        self.conn_plc = None
        self.sock_io = None
        self.ssock_io = None
        self.ssl_context = None

        self.conn_start = None

        self.start()

        self._error_q = Queue()

    def run(self):
        print('Starting', self.name)
        self._init_connections()
        self._start_connection_handler()
        while True:
            message = self.cfg.q_manage_in.get()
            if message == 'END':
                self._close_connections()
                return
            if message == '_Error':
                self._close_connections()
                self.cfg.q_manage_out.put(('Error', self))
                print('send error signal to manager')
                return

            print('Unknown message:', message)

    def _close_connections(self):
        self.conn_plc.close()
        self.sock_plc.close()
        self.sock_io.close()
        print('connections closed!')

    @staticmethod
    def _check_msg(msg: bytes):
        if msg == b'':
            raise Exception('connection closed')
        return msg

    def _start_connection_handler(self):
        # TODO: run this as process so you can kill externally?
        def handle_plc_incoming():
            self.ssock_io.settimeout(self.cfg.timeout)
            self.conn_plc.settimeout(self.cfg.timeout)
            try:
                while True:
                    # recv from plc and send to io
                    msg = Bridge._get_message(self.conn_plc)
                    print('PLC:', msg)
                    msg = self._check_msg(msg)
                    self._send_message(msg, 'remoteIO')

                    # recv from io and send to plc
                    msg = Bridge._get_message(self.ssock_io)
                    print('IO :', msg)
                    msg = self._check_msg(msg)
                    self._send_message(msg, 'plc')

            except socket.timeout:
                print('Socket timed out!')
                self.cfg.q_manage_in.put('_Error')

            except Exception as exception:
                print(exception)
                self.cfg.q_manage_in.put('_Error')

        Thread(target=handle_plc_incoming).start()

    @staticmethod
    def _get_header(sock: socket.socket) -> Tuple[bytes, int]:
        header = b''
        while len(header) < HEADER_SIZE:
            header += sock.recv(HEADER_SIZE - len(header))
        _trans_id, _prot_id, length = unpack('!HHH', header)
        return header, length

    @staticmethod
    def _get_payload(sock: socket.socket, length: int) -> bytes:
        payload = b''
        while len(payload) < length:
            payload += sock.recv(length - len(payload))
        return payload

    @staticmethod
    def _get_message(sock: socket.socket) -> bytes:
        header, length = Bridge._get_header(sock)
        payload = Bridge._get_payload(sock, length)
        return header + payload

    def _init_connections(self):
        self._init_io_connection()
        self._init_plc_connection()

    def _init_io_connection(self):
        self.sock_io = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock_io.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        try:
            if self.cfg.secure:
                self._init_ssl_context()
                self.ssock_io = self.ssl_context.wrap_socket(
                    self.sock_io, server_side=False, server_hostname=self.cfg.io_name)
                print('Connecting to remoteIO {0} ({1}:{2})'.format(
                    self.cfg.name, self.cfg.host_io, self.cfg.port_io))
                self.ssock_io.connect((self.cfg.host_io, self.cfg.port_io))
                print('Connected to remotIO in ssl using', self.ssock_io.version())
            else:
                self.ssock_io = self.sock_io
                print('Connecting to remoteIO', self.cfg.name)
                self.ssock_io.connect((self.cfg.host_io, self.cfg.port_io))
                print('Connected to remoteIO')
        except Exception as exc:
            print('Exception during connection establishment:')
            print(exc)
            self.cfg.q_manage_in.put('_Error')

    def _init_plc_connection(self):
        self.sock_plc = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock_plc.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.sock_plc.bind(('', self.cfg.port_plc))
        self.sock_plc.listen()
        print('waiting for plc to connect to', self.cfg.name)
        self.conn_plc, _addr = self.sock_plc.accept()
        print('plc connected')
        print('connected {0}:{1} to localhost:{2}'.format(_addr[0], _addr[1], self.cfg.port_plc))

    def _init_ssl_context(self):
        self.ssl_context = ssl.create_default_context(
            ssl.Purpose.SERVER_AUTH, cafile=self.cfg.io_cert)
        self.ssl_context.load_cert_chain(
            certfile=self.cfg.plc_cert, keyfile=self.cfg.plc_key)

    def _send_message(self, msg: bytes, dest: str):
        dest = dest.lower()
        if dest == 'remoteio':
            self.ssock_io.send(msg)
        elif dest == 'plc':
            self.conn_plc.send(msg)
        else:
            raise AttributeError('invalid destination')
