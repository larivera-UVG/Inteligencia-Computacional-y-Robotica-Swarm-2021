from gpiozero import Servo
from time import sleep

servo = Servo(2)
print("1. Rutina continua.")
print("2. Seleccionar angulo")
rutina = int(input("Seleccione la rutina a ejecutar: "))
try:

    while True:
        if rutina == 1:
            servo.min()
            sleep(0.5)
            servo.mid()
            sleep(0.5)
            servo.max()
            sleep(0.5)
            rutina = 0
        if rutina == 2:
            ang = float(input("Seleccione un angulo(-1,1): "))
            if ang > 1:
                rutina == 0   
            servo.value = ang
            
        rutina = int(input("Seleccione la rutina a ejecutar: "))
        print("1. Rutina continua.")
        print("2. Seleccionar angulo")
        
except KeyboardInterrupt:
        print("Program stopped")
