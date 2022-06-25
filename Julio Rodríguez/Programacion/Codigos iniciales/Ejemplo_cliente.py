import socket

# Create a TCP/IP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Connect the socket to the port where the server is listening
IP = '192.168.0.2'
PORT = 6000

server_address = (IP, PORT)
print('connecting to {} port {}'.format(*server_address))
sock.connect(server_address)

while True:
    
    try:
        while True:
            data = sock.recv(1024).decode("utf-8")
            if data == "1":
                print("Usted esta moviendo los motores")
            if data == "2":
                print("Usted esta mandando datos")
                message = f'Datos uwu'
                sock.send(message.encode("utf-8"))
    finally:
        print('closing socket')
        sock.close() 
