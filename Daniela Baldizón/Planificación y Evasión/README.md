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

### Guía para usar el algoritmo

En esta guía se mencionan los parámetros que se deben cambiar para poder probar el algoritmo.

El primer parámetro a modificar es el tamaño del mapa. La variable que controla esto es `grid_size`. Por ejemplo, si se quiere un mapa de 10x10 establecer esta variable en 10.

Luego, se pueden modificar los nodos de inicio y destino. Esto se hace con las variables `nodo_init` y `nodo_dest`, respectivamente. **El valor a asignar es tipo string y debe estar entre comillas simples ' '**. Es importante que estos valores asignados no excedan a la cantidad de casillas dentro del mapa, o puede tirar errores. 

En el código hay 7 mapas diferentes, en los comentarios está para qué es cada uno. Para cambiar de mapa se utilizó un switch case modificado por la variable `escenario`. Colocar esta variable en un número de 1 a 7 dependiendo el mapa a probar. Los obstáculos se establecen según el número de nodo, por ejemplo para un grid de 10x10, los obstáculos se establecerían de la siguiente manera: `obstaculos = [5,6,7,15,16]'. `

La cantidad máxima de iteraciones se modifica con la variable `t_max`. 

Se tienen 4 conjuntos de parámetros que pueden ser utilizados en el algoritmo. Para modificar cual usar se debe colocar la variable `constantes` en un número entre 1 y 4. 

El porcentaje de hormigas que se quiere con la misma solución se modifica con la variable `epsilon`. 

#### Ejemplo

En este ejemplo se colocaron los siguientes parámetros:

- `grid_size = 10;`
- `nodo_init = '1';`
- `nodo_dest = '100';`
- `escenario = 1;`
- `t_max = 50;`
- `constantes = 4;`
- `epsilon = 0.95;`

Los resultados obtenidos fueron los siguientes:

<img src = "https://user-images.githubusercontent.com/45132555/143379104-d8198c90-13d8-4f8e-89c3-3f8d7fdc96f9.jpg" width="400" height="400"> <img src = "https://user-images.githubusercontent.com/45132555/143379102-e2c57507-140e-4e4e-8709-8fc03cc88319.jpg" width="400" height="400">

La imagen de la izquierda es el mapa con la trayectoria entre los nodos. La imagen de la derecha son las gráficas de media y moda de los caminos encontrados.

Si se siguen los pasos antes mencionados y se obtiene un resultado similar al mostrado, el algoritmo está listo para hacer más pruebas.
