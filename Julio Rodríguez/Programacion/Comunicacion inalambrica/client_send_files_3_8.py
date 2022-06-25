import os
import socket
import struct
            
def sendPic(conn, filePath):
    print(filePath)
    pic = open(filePath, 'rb')
    chunk = pic.read(1024)
    while chunk:
        print("Sending Picture")
        conn.send(chunk)
        chunk = pic.read(1024)
    pic.close()
    print("Done sending")
    conn.close()
    return "Done sending"

def send_file(sck: socket.socket, filename):
    # Obtener el tamaño del archivo a enviar.
    filesize = os.path.getsize(filename)
    # Informar primero al servidor la cantidad
    # de bytes que serán enviados.
    sck.sendall(struct.pack("<Q", filesize))
    # Enviar el archivo en bloques de 1024 bytes.
    pic = open(filename, 'rb')
    chunk = pic.read(1024)
    while chunk:
        print("Sending Picture")
        sck.send(chunk)
        chunk = pic.read(1024)
    pic.close()
    print("Done sending")

with socket.create_connection(("192.168.0.3", 6190)) as conn:
    print("Conectado al servidor.")
    print("Enviando archivo...")
    mensaje = "Hola"
    conn.send(mensaje.encode("utf-8"))
    #send_file(conn, "video1.h264")
    #sendPic(conn,"test.jpg")
    print("Enviado.")
print("Conexión cerrada.")

