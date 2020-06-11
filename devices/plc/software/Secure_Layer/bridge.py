"""
This module contains the Bridge class.
"""
import socket
import ssl
from multiprocessing import Process, Queue
from struct import unpack
from threading import Thread
from time import sleep
from typing import Tuple

from config import Config

CONNECTION_TIMEOUT = 15
HEADER_SIZE = 6
MAX_PACKET_SIZE = 1024


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

    def run(self) -> None:
        print('[{}] Starting... '.format(self.name))
        self._init_connections()
        self._start_connection_handler()
        while True:
            message = self.cfg.q_manage_in.get()
            if message == 'END':
                self._close_connections()
                return
            else:
                print('[{}] Unknown message: {}'.format(self.name, message))

    def _close_connections(self) -> None:
        if self.conn_plc is not None:
            self.conn_plc.close()
        if self.sock_plc is not None:
            self.sock_plc.close()
        if self.sock_io is not None:
            self.sock_io.close()
        print('[{}] connections closed!'.format(self.name))

    def _error(self) -> None:
        self._close_connections()
        self.cfg.q_manage_out.put('Error')
        exit(1)

    @staticmethod
    def _check_msg(msg: bytes) -> None:
        if msg == b'':
            raise Exception('connection closed')
        return msg

    def _start_connection_handler(self) -> None:
        def handle_plc_incoming() -> None:
            self.ssock_io.settimeout(self.cfg.timeout)
            self.conn_plc.settimeout(self.cfg.timeout)
            try:
                while True:
                    # recv from plc and send to io
                    msg = Bridge._get_message(self.conn_plc)
                    # print('PLC:', msg)
                    msg = self._check_msg(msg)
                    self._send_message(msg, 'remoteIO')

                    # recv from io and send to plc
                    msg = Bridge._get_message(self.ssock_io)
                    # print('IO :', msg)
                    msg = self._check_msg(msg)
                    self._send_message(msg, 'plc')

            except socket.timeout:
                print('[{}] Socket timed out!'.format(self.name))
                self._error()

            except Exception as exception:
                print(exception)
                self._error()

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

    def _init_connections(self) -> None:
        self._init_io_connection()
        self._init_plc_connection()

    def _init_io_connection(self) -> None:
        self.sock_io = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock_io.settimeout(CONNECTION_TIMEOUT)
        self.sock_io.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        try:
            if self.cfg.secure:
                self._init_ssl_context()
                self.ssock_io = self.ssl_context.wrap_socket(
                    self.sock_io, server_side=False, server_hostname=self.cfg.io_name)
                print('[{}] Connecting to remoteIO ({}:{})'.format(
                    self.name, self.cfg.host_io, self.cfg.port_io))
                self.ssock_io.connect((self.cfg.host_io, self.cfg.port_io))
                print('[{}] Connected to remoteIO in ssl using {}'.format(
                    self.name, self.ssock_io.version()))
            else:
                self.ssock_io = self.sock_io
                print('[{}] Connecting to remoteIO'.format(self.name))
                self.ssock_io.connect((self.cfg.host_io, self.cfg.port_io))
                print('[{}] Connected to remoteIO'.format(self.name))
        
        except socket.timeout:
            print('[{}] socket timed out during connection establishment!'.format(self.name))
            self._error()
        except Exception as exc:
            print('[{}] Exception during connection establishment:'.format(self.name))
            print(exc)
            self._error()

    def _init_plc_connection(self) -> None:
        try:
            self.sock_plc = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.sock_plc.settimeout(CONNECTION_TIMEOUT)
            self.sock_plc.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            self.sock_plc.bind(('', self.cfg.port_plc))
            self.sock_plc.listen()
            print('[{}] waiting for plc to connect'.format(self.name))
            self.conn_plc, _addr = self.sock_plc.accept()
            print('[{}] plc connected'.format(self.name))
            print('[{}] connected {}:{} to localhost:{}'.format(
                self.name, _addr[0], _addr[1], self.cfg.port_plc))
        except socket.timeout:
            print('[{}] socket timed out during connection establishment!'.format(self.name))
            self._error()
        except Exception as exc:
            print('[{}] Exception during connection establishment:')
            print(exc)
            self._error()

    def _init_ssl_context(self) -> None:
        self.ssl_context = ssl.create_default_context(
            ssl.Purpose.SERVER_AUTH, cafile=self.cfg.io_cert)
        self.ssl_context.load_cert_chain(
            certfile=self.cfg.plc_cert, keyfile=self.cfg.plc_key)

    def _send_message(self, msg: bytes, dest: str) -> None:
        dest = dest.lower()
        if dest == 'remoteio':
            self.ssock_io.send(msg)
        elif dest == 'plc':
            self.conn_plc.send(msg)
        else:
            raise AttributeError('invalid destination')
