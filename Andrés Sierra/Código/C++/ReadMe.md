# Archivos en C++
## `pruebaACO.cpp`
Este archivo tiene el código para obtener la mejor ruta entre dos puntos de un grafo, utilizando las ecuaciones del algoritmo Ant System. Este es el código base que será ordenado dentro de una clase. Para inicializar se necesita definir varios parámetros `ITERACIONES`: la cantidad de veces que las hormigas calcularan la ruta. `NUMEROHORMIGAS`: cantidad de agentes que se tienen. `NUMERONODOS`: cantidad de nodos del grafo. Los parámetros `ALPHA, BETA, Q, RHO, TAUMAX` son parte de las ecuaciones del algoritmo y tienen el valor asignado con los cuales se obtuvieron mejores resultados. `NODOINICIAL y NODOFINAL`.
### Variables/punteros globales
`MEJORLONG`: contiene la mejor longitud de la ruta más corta. <br />
`numITER`: para cada hormiga guarda el número de iteraciones necesario para ejecutar el programa (funcionalidad, si el código se ejecuta más veces del necesario, el algoritmo puede agarrar memoria que no está definida o números no validos por lo que el algoritmo no funciona). <br />
`*MEJORRUTA`: contiene los nodos seleccionados por el algoritmo.  <br />
`**GRAFO`: asigno un espacio de memoria para cada nodo y para cada nodo tiene la información de los nodos disponibles (estando en un nodo indica a que nodos se puede mover la hormiga). <br />
`**RUTAS`:  asigno un espacio de memoria para cada hormiga y guardo las rutas escogidas de cada hormiga. <br />
`**COORDS`: asigno un espacio de memoria a cada nodo y guardo las coordenadas X y Y de cada uno. <br />
`**FEROMONAS`: para cada nodo guarda la cantidad de feromonas en cada nodo disponible.   <br />
`**deltaFEROMONAS`: = Q/l, parte de la ecuación y ayuda a definir la cantidad de feromona en cada nodo.  <br />
`**PROB`: guarda el valor dado por la ecuación de probabilidad PHI. <br />
### Funciones
| Función        |Descripción                                                  	|
|----------------|--------------------------------------------------------------|
|`void inicializar()`|Inicializa el algoritmo. Para cada puntero reserva los espacios de memoria necesarios en la memoria heap, para poder acceder a dichos espacios desde cualquier parte del Código. también asigna un valor inicial estas memorias para saber cuándo ya se han modificado. |          								
|`void conectarNODOS(int nodoi, int nodoj)`|Conecta los nodos de entrada asignando un 1 para indicar que el nodo está habilitado. Asigno primero el [nodoi][nodoj] y luego [nodoj][nodoi] para indicar que es un grafo bidireccional (me puedo mover del nodo i al j y también del j al i). |
|`void setCOORDENADAS(int nodo, double x, double y)`|Guardo la coordenadas X y Y de todos los nodos.| 
|`bool nodoVALIDO(int nodoi, int nodoc)`| Verifico que los nodos de entrada sean validos (revisa si en la memoria GRAFO hay un 1, indicando que si me puedo mover a ese nodo.|
|`double distancia(int nodoi, int nodoj)`|Calcula la distancia entre los dos nodos utilizando la ecuación euclidiana.  |
|`bool visitado(int hormigaK, int c)`|Verifico si el nodo C ya fue visitado por la hormigaK.|
|`double PHI(int nodoi, int nodoj, int hormigaK)`|Es la ecuación de probabilidad del algoritmo ANT SYSTEM. |
|`int siguienteNODO()`|Retorna el siguiente nodo que se moverá la hormiga. |
|`int nodoFINAL(int nodoi)`|Verifico las coordenadas x del nodo i y del nodo final para comprobar si la hormiga ya finalizo la ruta o tiene que seguir. |
|`void ruta(int hormigaK)`|Calcula la ruta de todas las hormigas comenzando en el nodo inicial. Si se cumplen todas las condiciones calcula la probabilidad y llama a la función `siguienteNODO()` para calcular a donde se moverá la hormiga. |
|`int valid(int hormigaK, int iteracion)`|Verifica si el nodo es válido, si se cumplen todas las condiciones y no hay ninguna condicion de paro retorna 0 y en el main se seguirá ejecutando el ciclo while.|
|`double longitud(int hormigaK, int numit)`|Con la ruta ya calculada de la hormigaK calcula la longitud de todo el recorrido llamando a la función `distancia`. Es importante el parámetro de entrada numit, este define las iteraciones que se ejecutara. Como ya se reservó el espacio de memoria, y se le asigno un valor inicial de -1 al puntero RUTAS, si se pasa de esta iteración intentara calcular la distancia con el "nodo= -1" lo cual causara un error. |
|`void actualizarFEROMONAS()`|Actualizo el valor de las feromonas en cada nodo para todas las hormigas|
|`void imprimirGRAFO()`|Despliego el grafo en la consola. |
|`void imprimirFEROMONAS()`|Despliego el valor de las feromonas en la consola. Tanto este como `imprimirGRAFO` no se recomienda utilizar cuando se tienen muchos nodos ya que la consola no es suficientemente grande para desplegar todos los valores lo cual dificulta la visualización.  |
|`void imprimirRESULTADOS()`|Imprime el resultado de la mejor ruta en la consola. En la visualización de los nodos seleccionados a la hora de imprimir se le suma 1 para que coincida con la definición del grafo en MATLAB y así poder visualizar de mejor manera la ruta seleccionada. |


### `main.cpp, ACO.cpp y ACO.h`
Estos tres archivos contienen la clase con algoritmo ACO descrito anteriormente.

`ACO.h` es el *header file*, aquí se definen todas las funciones y variables que se utilizar. Este archivo se divide en dos, la parte *public*, aquí se definen tanto variables como funciones que serán accedidos por el usuario (se utilizarán en el `main.cpp`). Y la parte *private* que forman parte de las operaciones internas del algoritmo. 

`ACO.cpp` aquí se define el contenido de todas las funciones tanto públicas como privadas.

`main.cpp` es el código principal, aquí se les asignan los valores a los parámetros se crea el grafo a utilizar con la función `conectarNODOS` y se define las coordenadas de los nodos con la función `setCOORDENADAS`. y se llama a la función `optimizar`.
| Función        |Descripción                                                  	|
|----------------|--------------------------------------------------------------|
|`void optimizar(int ITERACIONES)` | Para la clase se agrega esta función la cual se encarga de correr el proceso del algoritmo (lo que en `pruebaACO.cpp` se corría en el `void main()`. |

       	
Para compilar la clase se debe ejecutar el siguiente comando, este creara un archivo ejecutable en la carpeta donde está guardada la clase.
```bash
g++ main.cpp ACO.cpp -o main
```
Una vez con el ejecutable creado se corre utilizando
```bash
./main
```

### `pruebaRecepcion_UDP.cpp`
Coniente comunicación mediante un protocolo de comunicación UDP. Este cosido funciona en conjunto con y funciona como servidor, donde recibe y despliega la información. La información recibida proviene de Matlab utilizando como pruebas el código `PruebaComunicacion_UDP.m` ubicado en `Código/Matlab`.
