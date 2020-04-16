import socket
import ssl

listen_addr = '127.0.0.1'
listen_port = 8082
server_cert = 'server_certs/server.crt'
server_key = 'server_certs/server.key'
client_certs = 'client_certs/client.crt'

context = ssl.create_default_context(ssl.Purpose.CLIENT_AUTH)
context.verify_mode = ssl.CERT_REQUIRED
context.load_cert_chain(certfile=server_cert, keyfile=server_key)
context.load_verify_locations(cafile=client_certs)

bindsocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
bindsocket.bind((listen_addr, listen_port))
bindsocket.listen(5)

while True:
    print('Waiting for client')
    newsocket, fromaddr = bindsocket.accept()
    print('Client connected: {}:{}'.format(fromaddr[0], fromaddr[1]))
    # establish ssl
    conn = context.wrap_socket(newsocket, server_side=True)
    print('SSL established.')
    buf = b''  # Buffer to hold received client data
    try:
        while True:
            data = conn.recv(4096)
            if data:
                # Client sent us data. Append to buffer
                buf += data
            else:
                # No more data from client. Show buffer and close connection.
                print('Received:', buf)
                break
    except ConnectionResetError:
        print('Connection was closed by the remote host.')
    finally:
        print('Closing connection')
        conn.shutdown(socket.SHUT_RDWR)
        conn.close()