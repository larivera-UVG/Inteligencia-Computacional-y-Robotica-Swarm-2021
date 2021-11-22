## Planificación de trayectorias y evasión de obstáculos
En esta carpeta se encuentran los archivos utilizados en la implementación del algoritmo de Planificación de trayectorias y evasión de obstáculos. Asimismo, se encuentran los resultados obtenidos del barrido de parámetros y de las pruebas realizadas. Las carpetas se detallan a continuación:

### Matlab
En esta carpeta se encuentra el código de la implementación. El archivo PlanificacionEvasion.m es el archivo principal, y el resto de los archivos .m son funciones que se utilizan dentro del archivo prinicipal.

#### Código principal y funciones
- ant_decision.m

  Función utilizada para implementar el algoritmo 17.1 Artificial Ant Decision Process del libro Computational Intelligence an Introduction.
  
- findNode.m

  Esta función toma la información del grafo, con las coordenadas de algún nodo y devuelve a qué nodo corresponden las coordenadas.
  
- graph_grid.m

  Esta función crea un grafo cuadrado con nodos de 1 al número que el usuario desee. Por ejemplo graph_grid(10) crea 10x10 nodos de 1 a 10 vertical y horizontalmente. Esta         función   también guarda en el grafo el peso de cada enlace entre nodos (tau y eta).
  
- loop_remover.m

  Por ejemplo si tenemos los nodos: 1 2 3 8 5 6 3 4, loop_remover quita los números 8563 porque al inicio estaba en el nodo 3, luego se fue a nodos nada que ver y regresó al 3.   De este modo tenemos la ruta corta 1 2 3 4.
  
- MapasDibujados.m

  Esta función devuelve figuras con los mapas.

- PlanificacionEvasion.m

  Es el archivo principal y el cual debe ejecutarse. 
  
- rouletteWheel.m

  Esta función implementa la elección mediante rouletteWheel.

#### Archivos para el barrido de parámetros
- BarridoAlpha.m
- BarridoBeta.m
- BarridoGamma.m
- BarridoHormigas.m
- BarridoQ.m
- BarridoRho.m

  Estas funciones no requieren funciones adicionales a las que utiliza PlanificacionEvasion.m.
  
#### Archivos .mat
- webotsPath.mat

  Este archivo contiene los puntos de la trayectoria para utilizar en Webots.

### Resultados
En esta carpeta se encuentran los resultados de las pruebas realizadas en los distintos mapas. Estos resultados son las imágenes generadas y los datos generados en la ejecución del algoritmo. Asimismo, se encuentran los resultados de los barridos de parámetros. Los resultados de cada barrido se guardaron en una carpeta con el nombre correspondiente al valor de rho encontrado. El archivo barrido de parámetros contiene los resultados detallados de los barridos y el archivo resutlados contiene el resumen de los resultados por mapa.
