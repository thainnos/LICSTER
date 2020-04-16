import hashlib
import socket
import ssl
from struct import pack, unpack

from cryptography.hazmat.backends import default_backend
from cryptography.hazmat.primitives.asymmetric.ec import (ECDH, SECP256R1,
                                                          generate_private_key)
from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
from cryptography.hazmat.primitives.serialization import (Encoding,
                                                          PublicFormat,
                                                          load_pem_public_key)

HOST = 'localhost'
PORT = 200

def hash_cipher(shared_key):
    cipher = hashlib.sha256(shared_key).digest()[:16]
    print(f'cipher: {cipher.hex()}')
    return cipher

def handshake(sock):
    msg_len = unpack('!H', sock.recv(2))[0]
    message = sock.recv(msg_len)
    backend = default_backend()
    curve = SECP256R1()
    remote_public_key = load_pem_public_key(message, backend)
    
    private_key = generate_private_key(curve, backend)
    encoding, public_format = Encoding.PEM, PublicFormat.SubjectPublicKeyInfo
    public_key = private_key.public_key().public_bytes(encoding, public_format)

    msg_len = len(public_key)
    packed_len = pack('!H', msg_len)
    sock.sendall(packed_len)
    sock.sendall(public_key)

    shared_key = private_key.exchange(ECDH(), remote_public_key)
    print(f'shared key: {shared_key.hex()}')
    cipher = hash_cipher(shared_key)
    return cipher

def decrypt_message(msg: bytes, cipher_key: bytes, init_vector: bytes):
    algorithm = algorithms.AES(cipher_key)
    mode = modes.CBC(init_vector)
    cipher = Cipher(algorithm, mode, backend=default_backend())
    decryptor = cipher.decryptor()
    dec_msg = decryptor.update(msg) + decryptor.finalize()
    return dec_msg

def recv_message(sock, cipher):
    msg_len = unpack('!H', sock.recv(2))[0]
    msg = sock.recv(msg_len)
    content_len = unpack('!H', msg[:2])[0]
    init_vector = msg[2:18]
    enc_message = msg[18:]
    msg = decrypt_message(enc_message, cipher, init_vector)[:content_len]
    print(msg)
    return msg

def remote_io(host, port):
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
        sock.bind((host, port))
        sock.listen()
        conn, addr = sock.accept()
        print(f'Connection from {addr[0]}:{addr[1]}')
        cipher = handshake(conn)
        recv_message(conn, cipher)

if __name__ == "__main__":
    while True:
        remote_io(HOST, PORT)
