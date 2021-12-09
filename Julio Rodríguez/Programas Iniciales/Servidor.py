# Servidor
import socket

mi_socket = socket.socket()
mi_socket.bind( ('0.0.0.0',6000))
mi_socket.listen(5)
mensaje = "Saludos"
while True:
    conexion, addr= mi_socket.accept()
    print (socket.gethostname())
    print ("Nueva conexión establecida")
    print (addr)
    peticion = conexion.recv(1024).decode()
    print (peticion)
    conexion.send(mensaje.encode())
    conexion.close()
   
 
