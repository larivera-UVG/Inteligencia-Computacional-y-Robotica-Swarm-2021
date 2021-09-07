from gpiozero import PWMOutputDevice  
from gpiozero import DigitalOutputDevice
from time import sleep
#MOTOR 1
#Pin para el PWM1
PWM_izquierda = 2
#Motor izquiero movimiento hacia adelante
adelante_izquierdo = 3
#Motor izquiero movimiento hacia atras
atras_izquierdo = 4

#MOTOR 2
#Pin para el PWM2
PWM_derecha = 17
#Motor izquiero movimiento hacia adelante
adelante_derecha = 27
#Motor izquiero movimiento hacia atras
atras_derecha = 22

# Inicio de los PWM (pin utilizado, inicio del duty cicle y frecuencia en Hz iniciada)
Movimiento_Izquierda = PWMOutputDevice(PWM_izquierda, True, 0, 1000)
Movimiento_Derecha = PWMOutputDevice(PWM_derecha, True, 0, 1000)

#Inicio de los objetos para el controlador de los motores
Avance_Izquierda = DigitalOutputDevice(adelante_izquierdo)
Retroceso_Izquierda = DigitalOutputDevice(atras_izquierdo)

Avance_Derecha = DigitalOutputDevice(adelante_derecha)
Retroceso_Derecha = DigitalOutputDevice(atras_derecha)

def Alto():
    Avance_Izquierda.value = False
    Retroceso_Izquierda.value = False
    Avance_Derecha.value = False
    Retroceso_Derecha.value = False
    Movimiento_Izquierda = 0
    Movimiento_Derecha = 0


    




