## Registro de imagenes
En esta carpeta se encuentran los archivos utilizados en la implementación del algoritmo de registro de imágenes. Asimismo, se encuentran los resultados obtenidos de las pruebas realizadas. Las carpetas se detallan a continuación:

### Matlab

En esta carpeta se encuenta el código de la implementación y las funciones necesarias. 

#### Código principal y funciones
- ant_decision.m

  Función utilizada para implementar el algoritmo Medical Image Registration Based on an Improved Ant Colony Optimization Algorithm de Lin y Chang (2016).
  
- graph_grid.m

  Esta función crea un grafo cuadrado con nodos de 1 al número que el usuario desee. Por ejemplo graph_grid(10) crea 10x10 nodos de 1 a 10 vertical y horizontalmente. Esta         función también guarda en el grafo el peso de cada nodo (tau y eta). Esta función es diferente a la de los otros algoritmos. 
  
- loop_remover.m

  Por ejemplo si tenemos los nodos: 1 2 3 8 5 6 3 4, loop_remover quita los números 8563 porque al inicio estaba en el nodo 3, luego se fue a nodos nada que ver y regresó al 3.   De este modo tenemos la ruta corta 1 2 3 4.

- RegistroImagenes.m

  Es el archivo principal y el cual debe ejecutarse.
  
- rouletteWheel.m

  Esta función implementa la elección mediante rouletteWheel.


### Resultados

En esta carpeta se encuentran los resultados obtenidos en las pruebas del algoritmo. Aquí se encuentran las imágenes generadas y los archivos .mat con toda la información de las pruebas.

### Guia para usar el algoritmo

Para ejecutar se debe ingresar el nombre de las imágenes de referencia y template en las variables `Ref` y `Temp`, respectivamente. Las imágenes deben estar en la misma carpeta que los archivos .m.

La única variable que se modifica dentro del algoritmo es `tau_0`. Con esta variable se establece la cantidad de feromona inicial en el grafo.

#### Ejemplo

Para verificar si se tiene lo necesario para correr el algoritmo, comentar desde la línea 13 a la 49 y descomentar la sección que dice `Para hacer pruebas rápidas`. Se debería obtener un resultado como el siguiente:

![Im git](https://user-images.githubusercontent.com/45132555/143381110-ce6bfb54-2f01-4065-ad73-3f87e05d77e2.jpg)
