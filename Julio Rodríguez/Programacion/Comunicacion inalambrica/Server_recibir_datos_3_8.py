# -*- coding: utf-8 -*-
"""
Created on Wed Mar  2 15:37:39 2022

@author: Julio R
"""
# server.py
import socket
import struct

                
def recibir_archivo(sck: socket.socket, filename):
    picFile=open(filename,"wb")
    foto = conn.recv(1024)
    while foto:
        print("Recibiendo imagen")
        picFile.write(foto)
        foto = conn.recv(1024)
    picFile.close()
    
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
    
with socket.create_server(("192.168.0.3", 6190)) as server:
    print("Esperando al cliente...")
    conn, address = server.accept()
    print(f"{address[0]}:{address[1]} conectado.")
    print("Recibiendo archivo...")
    #recibir_archivo(conn, "test.jpg")
    receive_file(conn, "test.jpg")
    print("Archivo recibido.")
print("Conexión cerrada.")