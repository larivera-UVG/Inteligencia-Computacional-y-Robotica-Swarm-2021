# Implementación y Validación del Algoritmo de Robótica de Enjambre Particle Swarm Optimizationen Sistemas Físicos

## Indice
1. [Estructura de Carpetas](#Estructura-de-Carpetas)
2. [Antecedentes](#Antecedentes)
3. [Uso Raspberry Pi](#Raspberry-Pi)
4. [Documentos](#Documentos)

## Estructura de Carpetas
Estructura de las carpetas en el repositorio
```json
Codigo:
   | 
   └─── C
   |   |
   |   └─── PSO
   |   |
   |   └─── pruebaEnvioyRecepcion_1Hilo
   |   |
   |   └─── pruebaEnvioyRecepcion_2Hilos
   |   |
   |   └─── pruebaPSO_1agente
   |   |
   |   └─── pruebaSTRTOK
   |   |
   |   └─── prueba_comunicacionUDP_C&Matlab
   |
   |
   └─── Matlab
   |   |
   |   └─── prueba_comunicacionUDP_Matlab&C:
   |   |
   |   └─── prueba_enviodecoordenadas_Webots:
   |   |
   |   └─── Puntos de Evaluacion:
   |        |   
   |        └─── Archivos .mat y .txt con resultados de las simulaciones en Webots
   |
   └─── Python
       |
       └─── PSO3:
```

- Fases Anteriores
- Raspberry Pi
- Documentos

## Antecedentes 
### Aldo Aguilar (2019)
Tomando como base el algoritmo PSO como un planificador de trayectorias procedió a modificarlo para que este tome en consideración las dimensiones de robots físicosy la velocidad a la que estos pueden moverse, además se implementaron diferentes tipos decontroladores para buscar el robot pueda llegar al punto de meta realizando una trayectoriasuave y controlada.

### Juan Pablo Cahueque (2019)

### Eduardo Santizo (2020)
Diseñó una red neuronal recurrente (RNN) capaz de tomar diferentes métricas del PSO y en base a estas generar valores para los parámetros del algoritmo, q la red neuronal se le denominó PSO Tuner y permitió mejorar la velocidad y precisión del algoritmo estándar PSO significativamente. Finalmente, estos avances fueron compactados en un grupo de herramientas llamadoSwarm Robotics Toolboxpara ser usado en futuras fases.

### José Pablo Guerra (2020)

## Raspberry Pi
Para compilar algún código desde la terminar
```sh
gcc nombre_del_archivo.c -o _ejecutable_
```

Para compilar la librería  math agregar el comando 
```sh
-lm
```

Para compilar algún programa con multihilos agregar el comando 
```sh
-lpthread
```

Para apagar la Raspberry desde la terminal
```sh
sudo shutdown -h now
```

Para verificar si algún socket se encuentra abierto y forzar su terminación
```sh
ps ax | grep _ejecutable_
kill _num.ejecutable_
```

Para enviar algún archivo (o ejecutable) de una raspberry a otra (deben estar en la misma red)
```sh
scp _archivo_ pi@10.0.0.0:Desktop
```
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; <sub>Nota: modificar la direccion IP y la ubicacion de destino (ejemplo: Desktop).</sub>

Para conectarse desde la aplicación Cygwin (terminal) a alguna Raspberry 
```sh
ssh pi@10.0.0.0
```
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; <sub>Nota: modificar la direccion IP.</sub>

## Documentos
Se agrupan documentos importantes desarrollados en los cursos diseño e innovación en ingeniería y los preparativos finales para la defensa de la tesis.
