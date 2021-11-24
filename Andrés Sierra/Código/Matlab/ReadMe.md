# Archivos creados en Matlab
## `graph_grid.m`
Este archivo fue creado en la fase anterior por Gabriela Irriarte, es utilizado para generar un grafo con los nodos y conexiones. 
## `graficarRUTA.m`
Utiliza la función `graph_grid` para generar un grafo y poder visualizar la ruta seleccionada por el algoritmo ACO calculada en C++. Le asignamos manualmente los nodos generados en C++ y luego se grafica la ruta. En este repositorio en el directorio `Documentos/Resultados` podemos encontrar fotos donde se ven los resultados obtenidos utilizando este código. 

## `PruebaComunicacion_UDP.m`
Contiene comunicación mediante un socket utilizando el protocolo de comunicación UDP a una o varias direcciones IP conectados a una misma red. Este es el código base que se utiliza en rastreo por visión de computadora para enviar la información a los agentes que se tengan. Esta información vendrá desde Matlab y se enviara a los agentes que reciben utilizando C++. <br />

Se necesita tener la información concatenada en un string, utilizamos la función `strcat` donde vamos agregando la informacion que se necesita. <br />
Es necesario especificar la dirección IP a la cual se transmitirá la información y definir el puerto por el cual se estará comunicando. <br />

Con la función `udpport` se crea el socket para enviar la información. <br />

La variable `continueRunning` es utilizada como "bandera" para enviar la información, cada vez que sea *true*, se enviara la información. Es necesario escribir directamente en el *command window* de matlab la palabra *true* para que se envíen los datos.
