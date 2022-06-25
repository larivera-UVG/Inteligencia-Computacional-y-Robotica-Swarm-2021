import RPi.GPIO as GPIO

from gpiozero import Servo

from gpiozero import PWMOutputDevice
from gpiozero import DigitalOutputDevice

from flask import Flask, render_template, Response, request
from picamera import PiCamera
from gpiozero import LED
from threading import Condition
import time
import os
import io
import logging
import socketserver
from http import server

import socket
import struct

# Create a TCP/IP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
# Connect the socket to the port where the server is listening
IP = '192.168.0.7'
PORT = 6000

server_address = (IP, PORT)
print('connecting to {} port {}'.format(*server_address))
sock.connect(server_address)

#/////////////////Cofiguracion del Modulo de detección //////////////
# Use BCM GPIO references
# instead of physical pin numbers
GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)
# Define GPIO to use on Pi
GPIO_TRIGECHO = 14 
GPIO_TRIGECHO1 = 15


# Set pins as output and input
GPIO.setup(GPIO_TRIGECHO,GPIO.OUT)  # Initial state as output
GPIO.setup(GPIO_TRIGECHO1,GPIO.OUT)  # Initial state as output

# Set trigger to False (Low)
GPIO.output(GPIO_TRIGECHO, False)
GPIO.output(GPIO_TRIGECHO1, False)


#/////////////////Cofiguracion del Modulo de movimiento //////////////

#///////////////// Define Motor Driver GPIO Pins /////////////////
# Motor A, Left Side GPIO CONSTANTS
PWM_DRIVE_LEFT = 24		# ENA - H-Bridge enable pin
FORWARD_LEFT_PIN = 18	# IN1 - Forward Drive
REVERSE_LEFT_PIN = 23	# IN2 - Reverse Drive
# Motor B, Right Side GPIO CONSTANTS
PWM_DRIVE_RIGHT = 7		# ENB - H-Bridge enable pin
FORWARD_RIGHT_PIN = 8	# IN1 - Forward Drive
REVERSE_RIGHT_PIN = 25	# IN2 - Reverse Drive
STBY = 1
 
# Initialise objects for H-Bridge GPIO PWM pins
# Set initial duty cycle to 0 and frequency to 1000 and set Stanby as true
driveLeft = PWMOutputDevice(PWM_DRIVE_LEFT, True, 0, 1000)
driveRight = PWMOutputDevice(PWM_DRIVE_RIGHT, True, 0, 1000)

 
# Initialise objects for H-Bridge digital GPIO pins
STANBY = DigitalOutputDevice(STBY)
forwardLeft = DigitalOutputDevice(FORWARD_LEFT_PIN)
reverseLeft = DigitalOutputDevice(REVERSE_LEFT_PIN)
forwardRight = DigitalOutputDevice(FORWARD_RIGHT_PIN)
reverseRight = DigitalOutputDevice(REVERSE_RIGHT_PIN)
STANBY.value = True 

#///////////////// Modulo de vision por computadora /////////////////
#Especificaciones para la cámara
camera = PiCamera()
camera.resolution = (640, 480) #Resolución de la cámara
#camera.vflip = True    #Opción para voltear la cámara
camera.awb_mode = 'auto' #La cámara tiene diferentes filtros
                         #El filtro auto toma en cuenta las condiciones
                         #del espacio de grabación 

#Encabezado Stream 
PAGE="""\
<html>
<head>
<title>Raspberry Pi - Stream de prueba</title>
</head>
<body>
<center><h1>Raspberry Pi - Surveillance Camera</h1></center>
<center><img src="stream.mjpg" width="640" height="480"></center>
</body>
</html>
"""

class StreamingOutput(object):
    def __init__(self):
        self.frame = None
        self.buffer = io.BytesIO()
        self.condition = Condition()

    def write(self, buf):
        if buf.startswith(b'\xff\xd8'):
            # New frame, copy the existing buffer's content and notify all
            # clients it's available
            self.buffer.truncate()
            with self.condition:
                self.frame = self.buffer.getvalue()
                self.condition.notify_all()
            self.buffer.seek(0)
        return self.buffer.write(buf)

class StreamingHandler(server.BaseHTTPRequestHandler):
    def do_GET(self):
        if self.path == '/':
            self.send_response(301)
            self.send_header('Location', '/index.html')
            self.end_headers()
        elif self.path == '/index.html':
            content = PAGE.encode('utf-8')
            self.send_response(200)
            self.send_header('Content-Type', 'text/html')
            self.send_header('Content-Length', len(content))
            self.end_headers()
            self.wfile.write(content)
        elif self.path == '/stream.mjpg':
            self.send_response(200)
            self.send_header('Age', 0)
            self.send_header('Cache-Control', 'no-cache, private')
            self.send_header('Pragma', 'no-cache')
            self.send_header('Content-Type', 'multipart/x-mixed-replace; boundary=FRAME')
            self.end_headers()
            try:
                while True:
                    with output.condition:
                        output.condition.wait()
                        frame = output.frame
                    self.wfile.write(b'--FRAME\r\n')
                    self.send_header('Content-Type', 'image/jpeg')
                    self.send_header('Content-Length', len(frame))
                    self.end_headers()
                    self.wfile.write(frame)
                    self.wfile.write(b'\r\n')
            except Exception as e:
                logging.warning(
                    'Removed streaming client %s: %s',
                    self.client_address, str(e))
        else:
            self.send_error(404)
            self.end_headers()

class StreamingServer(socketserver.ThreadingMixIn, server.HTTPServer):
    allow_reuse_address = True
    daemon_threads = True

#Transmisión en vivo unicamente necesitando poner en el navegador
#la dirección wlan0 de la raspberry seguida de buffer escogido
#Ejemplo: 192.168.0.17:8000
def Stream():
    #output = StreamingOutput()
    camera.start_recording(output, format='mjpeg')
    try:
        address = ('', 8000)
        server = StreamingServer(address, StreamingHandler)
        server.serve_forever()
    finally:
        camera.stop_recording()
#Función para tomar foto y guardarla con el nombre test.jpg       
def Foto():
    camera.capture("test.jpg")
    
#Función para tomar video, la variable delay es la variable
#que determina el tiempo de video
def Video(delay):
    camera.start_recording("video1.h264")
    time.sleep(delay)
    camera.stop_recording()

#Función para enviar archivos, no importa si son
#imagenes o videos
def send_file(sck: socket.socket, filename):
    # Obtener el tamaño del archivo a enviar.
    filesize = os.path.getsize(filename)
    # Informar primero al servidor la cantidad
    # de bytes que serán enviados.
    sck.sendall(struct.pack("<Q", filesize))
    # Enviar el archivo en bloques de 1024 bytes.
    pic = open(filename, 'rb')
    chunk = pic.read(1024)
    print("Se esta enviando su archivo")
    while chunk:
        sck.send(chunk)
        chunk = pic.read(1024)
    pic.close()
    print("Archivo enviado")
           
#Funciones para el modulo de movimiento
#######################################
def allStop():
	forwardLeft.value = False
	reverseLeft.value = False
	forwardRight.value = False
	reverseRight.value = False
	driveLeft.value = 0
	driveRight.value = 0
 
def forwardDrive():
	forwardLeft.value = True
	reverseLeft.value = False
	forwardRight.value = True
	reverseRight.value = False
	driveLeft.value = 1.0
	driveRight.value = 1.0
 
def reverseDrive():
	forwardLeft.value = False
	reverseLeft.value = True
	forwardRight.value = False
	reverseRight.value = True
	driveLeft.value = 1.0
	driveRight.value = 1.0
 
def spinLeft():
	forwardLeft.value = False
	reverseLeft.value = True
	forwardRight.value = True
	reverseRight.value = False
	driveLeft.value = 1.0
	driveRight.value = 1.0
 
def SpinRight():
	forwardLeft.value = True
	reverseLeft.value = False
	forwardRight.value = False
	reverseRight.value = True
	driveLeft.value = 1.0
	driveRight.value = 1.0
 
def forwardTurnLeft():
	forwardLeft.value = True
	reverseLeft.value = False
	forwardRight.value = True
	reverseRight.value = False
	driveLeft.value = 0.2
	driveRight.value = 0.8
 
def forwardTurnRight():
	forwardLeft.value = True
	reverseLeft.value = False
	forwardRight.value = True
	reverseRight.value = False
	driveLeft.value = 0.8
	driveRight.value = 0.2
 
def reverseTurnLeft():
	forwardLeft.value = False
	reverseLeft.value = True
	forwardRight.value = False
	reverseRight.value = True
	driveLeft.value = 0.2
	driveRight.value = 0.8
 
def reverseTurnRight():
	forwardLeft.value = False
	reverseLeft.value = True
	forwardRight.value = False
	reverseRight.value = True
	driveLeft.value = 0.8
	driveRight.value = 0.2
	

#Funciones para el modulo de detección
#######################################
def measure():
    while True:
        GPIO.output(GPIO_TRIGECHO, True)
        time.sleep(0.00001)
        GPIO.output(GPIO_TRIGECHO, False)

        start = time.time()

        GPIO.setup(GPIO_TRIGECHO, GPIO.IN)
        while GPIO.input(GPIO_TRIGECHO)==0:
            start = time.time()

      # Wait for end of echo response
        while GPIO.input(GPIO_TRIGECHO)==1:
            stop = time.time()
        
        GPIO.setup(GPIO_TRIGECHO, GPIO.OUT)
        GPIO.output(GPIO_TRIGECHO, False)

        elapsed = stop-start
        distance = (elapsed * 34300)/2.0
        time.sleep(0.1)
        return distance

def measure1():
    while True:
        GPIO.output(GPIO_TRIGECHO1, True)
        time.sleep(0.00001)
        GPIO.output(GPIO_TRIGECHO1, False)

        start = time.time()

        GPIO.setup(GPIO_TRIGECHO1, GPIO.IN)
        while GPIO.input(GPIO_TRIGECHO1)==0:
            start = time.time()

      # Wait for end of echo response
        while GPIO.input(GPIO_TRIGECHO1)==1:
            stop = time.time()
        
        GPIO.setup(GPIO_TRIGECHO1, GPIO.OUT)
        GPIO.output(GPIO_TRIGECHO1, False)

        elapsed = stop-start
        distance = (elapsed * 34300)/2.0
        time.sleep(0.1)
        return distance

#Funciones disponibles para el Servomotor
##servo.min() #0 grados
##servo.mid() #90 grados
##servo.max() #180 grados
##servo.value = (-1,1)

while True:
    
    try:
        while True:
            data = sock.recv(1024).decode("utf-8")
            if data == "1":
                forwardDrive()
                #print("Motores hacia delante")
            if data == "2":
                reverseDrive()
                #print("Motores hacia atras")
            if data == "3":
                allStop()
                #print("Motores parados")
            if data == "4":
                forwardTurnLeft()
                #print("Giro a la izquierda")
            if data == "5":
                forwardTurnRight()
                #print("Giro a la derecha")
            if data == "6":
                distance = str(measure())
                distance1 = str(measure1())
                sock.send(distance.encode("utf-8"))
                sock.send(distance1.encode("utf-8"))
                #print ("  Distance of PWM1 : " + distance + "cm")
                #print ("  Distance of PWM1 : " + distance1 + "cm")
                time.sleep(1)
            if data == "7":
                # Define Servomotor
                servo = Servo(2)
                servo.min()
                time.sleep(0.5)
                servo.mid()
                time.sleep(0.5)
                servo.max()
                time.sleep(0.5)
            if data == "8":
                Foto()
                #print("Enviando imagen...")
                send_file(sock,"test.jpg")
                #print("Imagen enviada")
            if data == "9":
                Video(5)
                #print("Enviando video...")
                send_file(sock,"video1.h264")
                #print("Video enviado")                
            if data == "10":
                output = StreamingOutput()    
                Stream()   
    finally:
        #print('closing socket')
        sock.close() 
