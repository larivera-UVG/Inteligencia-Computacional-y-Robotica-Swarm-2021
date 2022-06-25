# -*- coding: utf-8 -*-
"""
Created on Sat Mar 26 13:13:58 2022

@author: Julio R
"""
import socket
import struct

# Create a UDP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
sock.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
                
server_address = ('192.168.0.255', 10000)
#sock.bind(server_address)
#print('starting up on {} port {}'.format(*server_address))


    
def receive_file_size(sock: socket.socket):
    # Esta función se asegura de que se reciban los bytes
    # que indican el tamaño del archivo que será enviado,
    # que es codificado por el cliente vía struct.pack(),
    # función la cual genera una secuencia de bytes que
    # representan el tamaño del archivo.
    fmt = "<Q"
    expected_bytes = struct.calcsize(fmt)
    received_bytes = 0
    stream = bytes()
    while received_bytes < expected_bytes:
        chunk = sock.recv(expected_bytes - received_bytes)
        stream += chunk
        received_bytes += len(chunk)
    filesize = struct.unpack(fmt, stream)[0]
    return filesize

def receive_file(sock: socket.socket, filename):
    # Leer primero del socket la cantidad de 
    # bytes que se recibirán del archivo.
    filesize = receive_file_size(sock)
    # Abrir un nuevo archivo en donde guardar
    # los datos recibidos.
    with open(filename, "wb") as f:
        received_bytes = 0
        # Recibir los datos del archivo en bloques de
        # 1024 bytes hasta llegar a la cantidad de
        # bytes total informada por el cliente.
        while received_bytes < filesize:
            chunk = sock.recv(1024)
            if chunk:
                f.write(chunk)
                received_bytes += len(chunk)    
    
while True:
    
   # print("Recibiendo archivo...")
    #recibir_archivo(conn, "test.jpg")
    #receive_file(sock, "video1.h264")
    #print("Archivo recibido.")
    #print("Conexión cerrada.")
    try:
        #print('connection from', address)
        
        # Receive the data in small chunks and retransmit it
        while True:
            #print("Escuchando")
            #raspberry, address = sock.recvfrom(4096)
            #print("Conectado a la rasp: {}".format(address))
            #print(raspberry)
            print("1. Hacía delante motores")
            print("2. Retroseso")
            print("3. Alto")
            print("4. Giro a la izquierda")
            print("5. Giro a la derecha")
            print("6. Medir sensores")
            print("7. Mover servo")
            print("8. Tomar foto ")   
            print("9. Tomar video")
            print("10. Realizar Stream ")
            print("11. Mandar datos a la siguiente Plataforma")
            mensaje = input('Ejecute un comando: ')
            sock.sendto(mensaje.encode("utf-8"),server_address)  
            if mensaje == "6":
                sensor0 = sock.recvfrom(1024)
                sensor1 = sock.recvfrom(1024)
                print("La distancia del sensor 1 es: "+ sensor0[0:6] + "cm")
                print("La distancia del sensor 2 es: "+ sensor1[0:6] + "cm \n")
            if mensaje == "8":
                receive_file(sock, "test.jpg")
                print("Foto recibida.\n")
            if mensaje == "9":
                receive_file(sock, "video.h264")
                print("Video recibido.\n")
            if mensaje == "11":
                sock.sendto(sensor0.encode("utf-8"),server_address) 
                sock.sendto(sensor1.encode("utf-8"),server_address)
                print("Datos enviados \n")

    finally:
        # Clean up the connection
        sock.close()  
