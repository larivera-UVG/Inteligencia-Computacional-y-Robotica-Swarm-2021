# -*- coding: utf-8 -*-
"""
Created on Thu Mar  3 15:37:56 2022

@author: Julio R
"""
# Servidor
import socket 
import struct
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
IP = '192.168.99.93'
PORT = 6000
s.bind( (IP, PORT))

s.listen(3)

def receive_file_size(sck: socket.socket):
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
        chunk = sck.recv(expected_bytes - received_bytes)
        stream += chunk
        received_bytes += len(chunk)
    filesize = struct.unpack(fmt, stream)[0]
    return filesize

def receive_file(sck: socket.socket, filename):
    # Leer primero del socket la cantidad de 
    # bytes que se recibirán del archivo.
    filesize = receive_file_size(sck)
    # Abrir un nuevo archivo en donde guardar
    # los datos recibidos.
    with open(filename, "wb") as f:
        received_bytes = 0
        # Recibir los datos del archivo en bloques de
        # 1024 bytes hasta llegar a la cantidad de
        # bytes total informada por el cliente.
        while received_bytes < filesize:
            chunk = sck.recv(1024)
            if chunk:
                f.write(chunk)
                received_bytes += len(chunk)        
    
while True:
    print('waiting for a connection')
    conexion, address= s.accept()
    print("Connection from " + str(address[0]) + " has been established.")
    
    try:
        print('connection from', address)
        
        # Receive the data in small chunks and retransmit it
        while True:
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
            mensaje = input(f'Ejecute un comando: ')
            conexion.send(mensaje.encode("utf-8"))  
            if mensaje == "6":
                sensor0 = conexion.recv(1024).decode("utf-8")
                sensor1 = conexion.recv(1024).decode("utf-8")
                print("La distancia del sensor 1 es: "+ sensor0[0:6] + "cm")
                print("La distancia del sensor 2 es: "+ sensor1[0:6] + "cm \n")
            if mensaje == "8":
                receive_file(conexion, "test.jpg")
                print("Foto recibida.\n")
            if mensaje == "9":
                receive_file(conexion, "video.h264")
                print("Video recibido.\n")

    finally:
        # Clean up the connection
        conexion.close()        
