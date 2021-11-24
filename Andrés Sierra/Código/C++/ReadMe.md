# Archivos en C++
## pruebaACO.cpp
Este archivo tiene el código para obtener la mejor ruta entre dos puntos de un grafo, utilizando las ecuaciones del algoritmo Ant System. Este es el código base que será ordenado dentro de una clase. Para inicializar se necisita definir varios paramentros `ITERACIONES`: la cantidad de veces que las hormigas calcularan la ruta. `NUMEROHORMIGAS`: cantidad de agentes que se tienen. `NUMERONODOS`: cantidad de nodos del grafo. Los parametros `ALPHA, BETA, Q, RO, TAUMAX` son parte de las ecuaciones del algoritmo y tienen el valor asignado con los cuales se obtuvierom mejores resultados. `NODOINICIAL y NODOFINAL`.
### Variables/punteros globales
`MEJORLONG`: contiene la mejor longitud de la ruta mas corta
`numITER`: para cada hormiga guarda el numero de iteraciones necesario para ejecutar el programa (funcionalidad, si el codigo se ejecuta mas veces del necesario, el algoritmo puede agarrar memoria que no esta definida o numeros no validos por lo que el algoritmo no funciona).
`*MEJORRUTA`: contiene los nodos seleccionados por el algoritmo
`**GRAFO`: para cada nodo, contiene que nodos estan disponibles (estando en un nodo indica a que nodos se puede mover la hormiga)
`**RUTAS`: contiene las rutas escogidas de todas las hormigas.
`**COORDS`: contiene las coordenadas de todos los nodos.
`**FEROMONAS`: para cada nodo guarda la cantidad de feromonas en cada nodo disponible.  
`**deltaFEROMONAS`: = Q/l, parte de la ecuacion y ayuda a definir la cantidad de feromona en cada nodo. 
`**PROB`: guarda el valor dado por la ecuacion de probabilidad PHI.
### Funciones
| Funcion        |Descripción                                                  	|
|----------------|--------------------------------------------------------------|
|`void inicializar()`|Crea              								|
|`void conectarNODOS(int nodoi, int nodoj)`|Reinicia el transceptor utilizando el pin de reset|
|`void setCOORDENADAS(int nodo, double x, double y)`|Inicializa | 
|`bool nodoVALIDO(int nodoi, int nodoc)`|Devuelve .|
|`double distancia(int nodoi, int nodoj)`|Devuelve |
|`bool visitado(int hormigaK, int c)`|f|
|`double PHI(int nodoi, int nodoj, int hormigaK)`|f|
|`int siguienteNODO()`|f|
|`int nodoFINAL(int nodoi)`|f|
|`void ruta(int hormigaK)`|f|
|`f`|f|
|`f`|f|
|`f`|f|
|`f`|f|
|`f`|f|
|`f`|f|
