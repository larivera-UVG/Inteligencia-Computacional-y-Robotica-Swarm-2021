# coding: utf-8
import time
import threading
from threading import Condition
import RPi.GPIO as GPIO
import os
import socket
import struct

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


with socket.create_connection(("192.168.0.3", 6190)) as conn:
    print("Conectado al servidor.")
    print("Enviando archivo...")
    distance = str(measure())
    distance1 = str(measure1())
    print ("  Distance of PWM1 : " + distance + "cm")
    print ("  Distance of PWM2 : " + distance1 + "cm")
    mensaje = "Hola"
    conn.send(distance.encode("utf-8"))
    conn.send(distance1.encode("utf-8"))
    print("Enviado.")
print("Conexión cerrada.")


