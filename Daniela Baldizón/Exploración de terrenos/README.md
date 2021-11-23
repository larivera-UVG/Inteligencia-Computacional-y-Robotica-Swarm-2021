## Exploración de terrenos
En esta carpeta se encuentran los archivos utilizados en la implementación del algoritmo de exploración de terrenos. Asimismo, se encuentran los resultados obtenidos. Las carpetas se detallan a continuación:

### Matlab
En esta carpeta se encuentra el código de la implementación. El archivo ExploraciónTerrenos.m es el archivo principal, y el resto de los archivos .m son funciones que se utilizan dentro del archivo prinicipal. Este programa utiliza la función parfor, que se encarga de paralelizar el algoritmo. 

#### Código principal y funciones
- ant_decision.m

  Función utilizada para implementar el algoritmo 17.1 Artificial Ant Decision Process del libro Computational Intelligence an Introduction.
  
- ExploracionTerrenos.m

  Es el archivo principal y el cual debe ejecutarse. Los obstáculos se establecen según el número de nodo, por ejemplo para un grid de 10x10 obstaculos = [5,6,7,15,16]'. Para     cambiar de mapa utlizar la variable escenario, tomar en cuenta que no todos son del mismo tamaño de grid. Modificar los nodos de destino e inicio con las variables nodo_dest y nodo_init, respectivamente. Este algoritmo está paralelizado, por lo que es recomendable darle a la         opción de start paralel pool de matlab antes de ejecutarlo, para que no se tarde tanto en empezar. Si esto último no se hace no hay problema ya que se activa solo al ejecutar.
  
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
