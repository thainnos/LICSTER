import socket
import ssl

host_addr = '127.0.0.1'
host_port = 8082
server_sni_hostname = 'plc.com'
server_cert = 'server_certs/server.crt'
client_cert = 'client_certs/client.crt'
client_key = 'client_certs/client.key'

context = ssl.create_default_context(ssl.Purpose.SERVER_AUTH, cafile=server_cert)
context.load_cert_chain(certfile=client_cert, keyfile=client_key)

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
    with context.wrap_socket(sock, server_side=False, server_hostname=server_sni_hostname) as ssock:
        ssock.connect((host_addr, host_port))
        print('SSL established.')
        print('Sending: "Hello, world!"')
        ssock.send(b'Hello, world!')
        print('Closing connection')
        ssock.close()
