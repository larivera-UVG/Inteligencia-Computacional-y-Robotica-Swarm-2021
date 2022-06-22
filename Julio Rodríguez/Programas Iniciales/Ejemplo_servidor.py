# -*- coding: utf-8 -*-
"""
Created on Tue Sep  7 11:24:55 2021

@author: Julio R
"""
# Servidor
import socket
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
IP = '192.168.0.2'
PORT = 6000
s.bind( (IP, PORT))

s.listen(1)


while True:
    print('waiting for a connection')
    conexion, address= s.accept()
    print("Connection from " + str(address[0]) + " has been established.")
    
    try:
        print('connection from', address)
        
        # Receive the data in small chunks and retransmit it
        while True:
            print("1. Hacía delante motores")
            print("2. Recibir información")
            mensaje = input(f'Ejecute un comando: ')
            conexion.send(mensaje.encode("utf-8"))
            if mensaje == "2":
                data = conexion.recv(1024).decode("utf-8")
                print('received {!r}'.format(data))
    finally:
        # Clean up the connection
        conexion.close()        
