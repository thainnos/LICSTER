import hashlib
import socket
from os import urandom
from struct import pack, unpack
from threading import Thread

from cryptography.hazmat.primitives.asymmetric.ec import (ECDH,
                                                          generate_private_key)
from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
from cryptography.hazmat.primitives.serialization import (Encoding,
                                                          PublicFormat,
                                                          load_pem_public_key)

from context import Context
from queue import Empty, Queue


class Layer(Thread):
    
    def __init__(self, ctx: Context):
        super(Layer, self).__init__()
        self.name = 'Layer-' + ctx.name
        self.ctx = ctx

        # start own Thread
        self.start()

        self.io_thread = None
        self.plc_thread = None

        self.error_q = Queue()

    def run(self):
        try:
            self.create_connections()
        except Exception as e:
            self.ctx.q_manage_out.put(('Error', self))
            print(e)
            return

        self.start_connection_handler()

        while True:
            message = self.ctx.q_manage_in.get()
            if message == 'END':
                return
            elif message == '_Error':
                self.ctx.q_manage_out.put(('Error', self))
                return

    def create_connections(self):
        self._handshake_remoteIO()
        self._hash_cipher()
        self._handshake_plc()
        
    def start_connection_handler(self):
        # TODO: still awaits one connection after error?
        # TODO: this is still wrong, need send_plc_message!
        def handle_io_incoming():
            while True:
                try:
                    self.ctx.q_io_in.put(self.recv_io_message())
                except:
                    self.ctx.q_manage_in.put('_Error')
                    self.error_q.put('Error')
                    return
                try:
                    msg = self.error_q.get_nowait()
                    if msg == 'Error':
                        return
                except Empty:
                    continue

        def handle_plc_incoming():
            while True:
                try:
                    self.send_io_message(self.recv_plc_message())
                except:
                    self.ctx.q_manage_in.put('_Error')
                    self.error_q.put('Error')
                    return
                try:
                    msg = self.error_q.get_nowait()
                    if msg == 'Error':
                        return
                except Empty:
                    continue
        
        Thread(target=handle_plc_incoming).start()
        Thread(target=handle_io_incoming).start()


    def _handshake_remoteIO(self):
        # private key
        private_key = generate_private_key(self.ctx.curve, self.ctx.backend)

        # create formatted public key
        encoding, public_format = Encoding.PEM, PublicFormat.SubjectPublicKeyInfo
        public_key = private_key.public_key().public_bytes(encoding, public_format)

        # pack length of message in uint16_t
        packed_len = pack('!H', len(public_key))

        # establish connection
        io_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.ctx.socket_io = io_sock
        io_sock.connect((self.ctx.host_io, self.ctx.port_io))

        # send message length and message
        io_sock.sendall(packed_len)
        io_sock.sendall(public_key)

        # recieve message length
        msg_len = unpack('!H', io_sock.recv(2))[0]

        # recieve message
        message = io_sock.recv(msg_len)
        remote_public_key = load_pem_public_key(message, self.ctx.backend)

        # create shared key
        shared_key = private_key.exchange(ECDH(), remote_public_key)
        self.ctx.shared_key = shared_key
        # print(f'shared key: {shared_key.hex()}')

    def _handshake_plc(self):
        print('starting plc socket...')

        # prepare socket
        socket_type = 'AF_UNIX' if hasattr(socket, 'AF_UNIX') else 'AF_INET'
        # print('Decided on', socket_type)
        plc_sock = socket.socket(getattr(socket, socket_type), socket.SOCK_STREAM)
        plc_sock.bind(('', self.ctx.port_plc))
        plc_sock.listen()
        conn, addr = plc_sock.accept()
        print('plc connected!')
        self.ctx.conn_plc = conn
        print(f'connected {addr[0]}:{addr[1]} to localhost:{self.ctx.port_plc}')

    # TODO: accept a parameter
    def _hash_cipher(self):
        cipher = hashlib.sha256(self.ctx.shared_key).digest()[:16]
        self.ctx.cipher = cipher
        # print(f'cipher: {cipher.hex()}')

    def _encrypt_message(self, msg: bytes):
        # if message length is not multiple of 16 pad it with b'\0's
        content_len = pack('!H', len(msg))  
        if (len(msg) % 16) > 0:
            msg += (16 - (len(msg) % 16)) * b'\0'

        cipher_key = self.ctx.cipher_io
        algorithm = algorithms.AES(cipher_key)
        init_vector = urandom(16)
        mode = modes.CBC(init_vector)
        cipher = Cipher(algorithm, mode, backend=self.ctx.backend)
        encryptor = cipher.encryptor()
        encrypted_message = encryptor.update(msg) + encryptor.finalize()
        # TODO: maybe turn this into a remove n bytes message > only 4 bits in size
        payload = content_len + init_vector + encrypted_message
        return payload

    def send_io_message(self, msg: bytes):
        # encrypt message
        enc_msg = self._encrypt_message(msg)
        
        # pack message length
        packed_len = pack('!H', len(enc_msg))

        # send packed length and message block
        self.ctx.socket_io.sendall(packed_len)
        self.ctx.socket_io.sendall(enc_msg)

    # TODO: Check if this works
    def _decrypt_message(self, msg:bytes, init_vector: bytes):
        
        cipher_key = self.ctx.cipher_io
        algorithm = algorithms.AES(cipher_key)
        mode = modes.CBC(init_vector)
        cipher = Cipher(algorithm, mode, backend=self.ctx.backend)
        decryptor = cipher.decryptor()
        dec_msg = decryptor.update(msg) + decryptor.finalize()
        return dec_msg

    # TODO: Check if this works
    def recv_io_message(self):
        # recieve message
        msg_len = unpack('!H', self.ctx.socket_io.recv(2))[0]
        msg = self.ctx.socket_io.recv(msg_len)
        content_len = unpack('!H', msg[:2])[0]
        init_vector = msg[2:18]
        enc_message = msg[18:]

        # truncate padding
        msg = self._decrypt_message(enc_message, init_vector)[:content_len]
        print(msg)
        return msg

    def recv_plc_message(self):
        sock = self.ctx.conn_plc
        # TODO: figure out packet size
        msg = sock.recv(1024)
        print('plc msg:', msg)
        return msg

    # TODO: Implement this!
    def send_plc_message(self, msg: bytes):
        pass
