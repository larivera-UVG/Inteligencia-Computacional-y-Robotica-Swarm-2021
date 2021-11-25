## Exploración de terrenos
En esta carpeta se encuentran los archivos utilizados en la implementación del algoritmo de exploración de terrenos. Asimismo, se encuentran los resultados obtenidos. Las carpetas se detallan a continuación:

![bduck-bduckbjones](https://user-images.githubusercontent.com/45132555/143373409-23693018-9744-4268-8d08-0b6c0be35ab2.gif)

### Matlab
En esta carpeta se encuentra el código de la implementación. El archivo ExploraciónTerrenos.m es el archivo principal, y el resto de los archivos .m son funciones que se utilizan dentro del archivo prinicipal. Este programa utiliza la función parfor, que se encarga de paralelizar el algoritmo. 

#### Código principal y funciones
- ant_decision.m

  Función utilizada para implementar el algoritmo 17.1 Artificial Ant Decision Process del libro Computational Intelligence an Introduction.
  
- ExploracionTerrenos.m

  Es el archivo principal y el cual debe ejecutarse. 
  
- findNode.m

  Esta función toma la información del grafo, con las coordenadas de algún nodo y devuelve a qué nodo corresponden las coordenadas.
  
- graph_grid.m

  Esta función crea un grafo cuadrado con nodos de 1 al número que el usuario desee. Por ejemplo graph_grid(10) crea 10x10 nodos de 1 a 10 vertical y horizontalmente. Esta         función   también guarda en el grafo el peso de cada enlace entre nodos (tau y eta).
  
- loop_remover.m

  Por ejemplo si tenemos los nodos: 1 2 3 8 5 6 3 4, loop_remover quita los números 8563 porque al inicio estaba en el nodo 3, luego se fue a nodos nada que ver y regresó al 3.   De este modo tenemos la ruta corta 1 2 3 4.
  
- rouletteWheel.m

  Esta función implementa la elección mediante rouletteWheel.

### Resultados
En esta carpeta se encuentran los resultados de las corridas realizadas en los distintos mapas. Estos resultados son las imágenes generadas y los datos generados en la ejecución del algoritmo. 

### Guía para usar el algoritmo

En esta guía se mencionan los parámetros que se deben cambiar para poder probar el algoritmo.

El primer parámetro a modificar es el tamaño del mapa. La variable que controla esto es `grid_size`. Por ejemplo, si se quiere un mapa de 10x10 establecer esta variable en 10.

Luego, se pueden modificar los nodos de inicio y destino. Esto se hace con las variables `nodo_init` y `nodo_dest`, respectivamente. **El valor a asignar es tipo string y debe estar entre comillas simples ' '**. Es importante que estos valores asignados no excedan a la cantidad de casillas dentro del mapa, o puede tirar errores. 

En el código hay 7 mapas diferentes, en los comentarios está para qué es cada uno. Para cambiar de mapa se utilizó un switch case modificado por la variable `escenario`. Colocar esta variable en un número de 1 a 7 dependiendo el mapa a probar. Los obstáculos se establecen según el número de nodo, por ejemplo para un grid de 10x10, los obstáculos se establecerían de la siguiente manera: `obstaculos = [5,6,7,15,16]'. `

La cantidad máxima de iteraciones se modifica con la variable `t_max`. El porcentaje de hormigas que se quiere con la misma solución se modifica con la variable `epsilon`. 

Este algoritmo está paralelizado, por lo que es recomendable darle a la opción de start paralel pool de matlab antes de ejecutarlo, para que no se tarde tanto en empezar. Si esto último no se hace no hay problema ya que se activa solo al ejecutar.

#### Ejemplo

En este ejemplo se colocaron los siguientes parámetros:

- `grid_size = 10;`
- `nodo_init = '1';`
- `nodo_dest = '100';`
- `escenario = 1;`
- `t_max = 150;`
- `epsilon = 0.95;`

Los resultados obtenidos fueron los siguientes:

<img src = "https://user-images.githubusercontent.com/45132555/143378126-6c3c8899-09d6-4842-bda3-fdf14fbc1f39.jpg" width="400" height="400"> <img src = "https://user-images.githubusercontent.com/45132555/143378131-868323e0-2cbc-4c31-8aff-73c9d1b0500b.jpg" width="400" height="400">

La imagen de la izquierda es el mapa con la trayectoria entre los nodos. La imagen de la derecha es el mapa obtenido luego de la exploración.

Si se siguen los pasos antes mencionados y se obtiene un resultado similar al mostrado, el algoritmo está listo para hacer más pruebas.

