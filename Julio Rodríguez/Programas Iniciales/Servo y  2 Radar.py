# coding: utf-8
import time
import threading
from threading import Condition
from gpiozero import Servo
import RPi.GPIO as GPIO

# Use BCM GPIO references
# instead of physical pin numbers
GPIO.setmode(GPIO.BCM)

#Pin para servomotor
servo = Servo(2)

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

numero = int(input("Seleccione una opcion: "))
while True:

    if numero == 1:
        servo.min()
        time.sleep(0.1)
        distance = measure()
        distance1 = measure1()
        print ("  Distance of PWM1 : %.1f cm" % distance)
        print ("  Distance of PWM2 : %.1f cm" % distance1)
        time.sleep(1)
        numero = 0

    if numero == 2:
        servo.mid()
        time.sleep(0.1)
        distance = measure()
        distance1 = measure1()
        print ("  Distance of PWM1 : %.1f cm" % distance)
        print ("  Distance of PWM2 : %.1f cm" % distance1)
        time.sleep(1)
        numero = 0

    if numero == 3:
        servo.max()
        time.sleep(0.1)
        distance = measure()
        distance1 = measure1()
        print ("  Distance of PWM1 : %.1f cm" % distance)
        print ("  Distance of PWM2 : %.1f cm" % distance1)
        time.sleep(1)
        numero = 0

    else:
        numero = int(input("Seleccione una opcion: "))

