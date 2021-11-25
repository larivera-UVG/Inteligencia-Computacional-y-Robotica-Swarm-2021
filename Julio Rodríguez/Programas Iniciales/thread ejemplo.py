import threading
import time
from gpiozero import PWMOutputDevice  
from gpiozero import DigitalOutputDevice
from time import sleep
from flask import Flask, render_template, Response, request
from picamera import PiCamera
from gpiozero import LED
from gpiozero import MotionSensor
from threading import Condition
import os
import io
import logging
import socketserver
from http import server
import RPi.GPIO as GPIO
# Define GPIO to use on Pi
GPIO_TRIGECHO = 15
#Motor A izquierdo
PWM_DRIVE_LEFT = 21
# Motor izquierdo hacia adelante 
FORWARD_LEFT_PIN = 26	
# Motor izquierdo hacia atras  
REVERSE_LEFT_PIN = 19
# Initialise objects for H-Bridge GPIO PWM pins  
# Inicio del PWM, el pin utilizado, inicio del duty cicle y frecuencia en Hz iniciada
driveLeft = PWMOutputDevice(PWM_DRIVE_LEFT, True, 0, 1000)  
# Initialise objects for H-Bridge digital GPIO pin
forwardLeft = DigitalOutputDevice(FORWARD_LEFT_PIN)  
reverseLeft = DigitalOutputDevice(REVERSE_LEFT_PIN)

GPIO.setup(GPIO_TRIGECHO,GPIO.OUT)  # Initial state as output
# Set trigger to False (Low)
GPIO.output(GPIO_TRIGECHO, False)

camera = PiCamera()
camera.resolution = (640, 480)
camera.vflip = True
camera.awb_mode = 'auto'


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

camera.awb_mode = 'auto'

def allStop():
    forwardLeft.value = False  	
    reverseLeft.value = False  	 	
    driveLeft.value = 0
    driveLeft.value = 0
    
def forwardDrive():
    forwardLeft.value = True  	
    reverseLeft.value = False  	
    driveLeft.value = 1.0    
 

def reverseDrive():
    forwardLeft.value = False  	
    reverseLeft.value = True	
    driveLeft.value = 1.0  	
    
def spinLeft():
    forwardLeft.value = False
    reverseLeft.value = True  	 	
    driveLeft.value = 1.0  
 
def forwardTurnLeft():  	
    forwardLeft.value = True  	
    reverseLeft.value = False  	 	
    driveLeft.value = 0.2  	
    
def reverseTurnLeft():  	
    forwardLeft.value = False  	
    reverseLeft.value = True	
    driveLeft.value = 0.8 	

def thread_delay(thread_name, delay):
    count = 0
    while count < 3:
        time.sleep(delay)
        count += 1
        print(thread_name, '-------->', time.time())

def stream():
    #output = StreamingOutput()
    camera.start_recording(output, format='mjpeg')
    try:
        address = ('', 8000)
        server = StreamingServer(address, StreamingHandler)
        server.serve_forever()
    finally:
       camera.stop_recording()

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

        print ("  Distancia : %.1f cm" % distance)
        time.sleep(1)
#        return distance


t1 = threading.Thread(target=stream)
t2 = threading.Thread(target=forwardDrive)
t3 = threading.Thread(target=measure)
output = StreamingOutput()
t1.start()
t2.start()
t3.start()
t1.join()
t2.join()
t3.join()
