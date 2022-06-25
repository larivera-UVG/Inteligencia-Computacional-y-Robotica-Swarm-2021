# coding: utf-8
import time
import threading
from threading import Condition
import RPi.GPIO as GPIO
from gpiozero import PWMOutputDevice
from gpiozero import DigitalOutputDevice
from time import sleep
# Servidor
import socket

# Use BCM GPIO references
# instead of physical pin numbers
GPIO.setmode(GPIO.BCM)

# Define GPIO to use on Pi
GPIO_TRIGECHO = 14
GPIO_TRIGECHO1 = 15
print ("Sensor Ultrasonico PWM1 y PWM2")

# Set pins as output and input
GPIO.setup(GPIO_TRIGECHO,GPIO.OUT)  # Initial state as output
GPIO.setup(GPIO_TRIGECHO1,GPIO.OUT)  # Initial state as output

# Set trigger to False (Low)
GPIO.output(GPIO_TRIGECHO, False)
GPIO.output(GPIO_TRIGECHO1, False)

#///////////////// Define Motor Driver GPIO Pins /////////////////
# Motor A, Left Side GPIO CONSTANTS
PWM_DRIVE_LEFT = 21		# ENA - H-Bridge enable pin
FORWARD_LEFT_PIN = 20	# IN1 - Forward Drive
REVERSE_LEFT_PIN = 16	# IN2 - Reverse Drive
# Motor B, Right Side GPIO CONSTANTS
PWM_DRIVE_RIGHT = 1		# ENB - H-Bridge enable pin
FORWARD_RIGHT_PIN = 8	# IN1 - Forward Drive
REVERSE_RIGHT_PIN = 7	# IN2 - Reverse Drive
STBY = 12
 
# Initialise objects for H-Bridge GPIO PWM pins
# Set initial duty cycle to 0 and frequency to 1000
driveLeft = PWMOutputDevice(PWM_DRIVE_LEFT, True, 0, 1000)
driveRight = PWMOutputDevice(PWM_DRIVE_RIGHT, True, 0, 1000)
 
# Initialise objects for H-Bridge digital GPIO pins
STANBY = DigitalOutputDevice(STBY)
forwardLeft = DigitalOutputDevice(FORWARD_LEFT_PIN)
reverseLeft = DigitalOutputDevice(REVERSE_LEFT_PIN)
forwardRight = DigitalOutputDevice(FORWARD_RIGHT_PIN)
reverseRight = DigitalOutputDevice(REVERSE_RIGHT_PIN)
 
mi_socket = socket.socket()
mi_socket.bind( ('0.0.0.0',6000))
mi_socket.listen(5)
#Habilitamos el puente de H 
STANBY.value = True

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
        distance1 = (elapsed * 34300)/2.0
        time.sleep(0.1)
        return distance1

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
    
try:

    while True:
        conexion, addr= mi_socket.accept()
        print (socket.gethostname())
        print ("Nueva conexión establecida")
        print (addr)
        peticion = conexion.recv(1024).decode()
        if peticion == "1":
            distance = measure()
            distance1 = measure1()
            mensaje = "  Distance of PWM1 : %.1f cm" % distance
            mensaje1 = "  Distance of PWM2 : %.1f cm" % distance1
            conexion.send(mensaje.encode())
            conexion.send(mensaje1.encode())
            time.sleep(1)
            forwardDrive()
            time.sleep(0.5)
            peticion = 0
        if peticion == "2":
            distance = measure()
            distance1 = measure1()
            mensaje = "  Distance of PWM1 : %.1f cm" % distance
            mensaje1 = "  Distance of PWM2 : %.1f cm" % distance1
            conexion.send(mensaje.encode())
            conexion.send(mensaje1.encode())
            time.sleep(1)
            reverseDrive()
            time.sleep(2)
            petición = 0
        if peticion == "3":
            allstop()
            
        

except KeyboardInterrupt:
    print("Stop")
    GPIO.cleanup()
