# Implementación y Validación del Algoritmo de Robótica de Enjambre Particle Swarm Optimizationen Sistemas Físicos

## Indice
1. [Estructura de Carpetas](#Estructura-de-Carpetas)
2. [Antecedentes](#Antecedentes)
3. [Codigo](#Codigo)
4. [Documentos](#Documentos)
5. [Uso Raspberry Pi](#Raspberry-Pi)

## Estructura de Carpetas
Estructura de las carpetas en el repositorio
```json
Repositorio
| 
└─── Codigo:
|    | 
|    └─── C
|    |   |
|    |   └─── PSO
|    |   |
|    |   └─── pruebaEnvioyRecepcion_1Hilo
|    |   |
|    |   └─── pruebaEnvioyRecepcion_2Hilos
|    |   |
|    |   └─── pruebaPSO_1agente
|    |   |
|    |   └─── pruebaSTRTOK
|    |   |
|    |   └─── prueba_comunicacionUDP_C&Matlab
|    |
|    |
|    └─── Matlab
|    |   |
|    |   └─── prueba_comunicacionUDP_Matlab&C:
|    |   |
|    |   └─── prueba_enviodecoordenadas_Webots:
|    |   |
|    |   └─── Puntos de Evaluacion:
|    |        |   
|    |        └─── Archivos .mat y .txt con resultados de las simulaciones en Webots
|    |
|    └─── Python
| 	 |
| 	 └─── PSO3
| 	   
| 
└─── Documentos:
|    | 
|    └─── Articulo
|    | 
|    └─── Imagenes
|    | 
|    └─── Protocolo
|    | 
|    └─── Tesis 
|
|
└─── Raspberry Pi:
     | 
     └─── Guias de instalacion
	   
```

## Antecedentes 
### Aldo Aguilar (2019)
Tomando como base el algoritmo PSO como un planificador de trayectorias procedió a modificarlo para que este tome en consideración las dimensiones de robots físicosy la velocidad a la que estos pueden moverse, además se implementaron diferentes tipos decontroladores para buscar el robot pueda llegar al punto de meta realizando una trayectoriasuave y controlada.

### Juan Pablo Cahueque (2019)

### Eduardo Santizo (2020)
Diseñó una red neuronal recurrente (RNN) capaz de tomar diferentes métricas del PSO y en base a estas generar valores para los parámetros del algoritmo, q la red neuronal se le denominó PSO Tuner y permitió mejorar la velocidad y precisión del algoritmo estándar PSO significativamente. Finalmente, estos avances fueron compactados en un grupo de herramientas llamadoSwarm Robotics Toolboxpara ser usado en futuras fases.

### José Pablo Guerra (2020)


## Código
Dentro de la carpeta de Código se encuentran todos los código desarrollados para esta tesis, se agrupan de acuerdo con el  lenguaje de programación usado y se da una breve descripción de su funcionamiento y de cómo ejecutarlos, así como la razón de su creación (validación de la prueba).    


## Documentos
Dentro de la carpeta de Documentos se agrupan documentos importantes desarrollados en los cursos diseño e innovación en ingeniería y los preparativos finales para la defensa de la tesis.


## Raspberry Pi
Dentro de la carpeta de Documentos se agrupan documentos importantes para el uso del ordenador Raspberry Pi, usado para la elaboración de esta tesis. Comandos útiles y notas para compilar programas



