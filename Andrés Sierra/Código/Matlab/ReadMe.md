
# Archivos creados en Matlab
## `graph_grid.m`
Este archivo fue creado en la fase anterior por Gabriela Irriarte, es utilizado para generar un grafo con los nodos y conexiones. 
## `graficarRUTA.m`
Utiliza la función `graph_grid` para generar un grafo y poder visualizar la ruta seleccionada por el algoritmo ACO en C++. Le asignamos manualmente los nodos generados en C++ y luego se grafica la ruta. En este repositorio en el directorio `Documentos/Resultados` podemos encontrar fotos donde se ven los resultados obtenidos utilizando este Código. 

## `PruebaComunicacion_UDP.m`
Contiene comunicación mediante un socket utilizando el protocolo de comunicación UDP a una o varias direcciones IP conectados a una misma red. Este es el código base que se utiliza en rastreo por visión de computadora para enviar la información a los agentes que se tengan. Esta información se enviará desde Matlab a todos los agentes que reciben utilizando C++. Tiene la función de cliente, donde se agrupa y envía la información a direcciones específicas. <br />

Se necesita tener la información concatenada en un string, utilizamos la función `strcat` donde vamos agregando la informacion que se necesita. <br />
Es necesario especificar la dirección IP a la cual se transmitirá la información y definir el puerto por el cual se estará comunicando. <br />

Para Matlab 2021: con la función `udpport` se crea el socket para enviar la información. <br />

Para Matlab 2019: se utiliza la función `udp`

La variable `continueRunning` es utilizada como "bandera" para enviar la información, cada vez que sea *true*, se enviara la información. Es necesario escribir directamente en el *command window* de Matlab la palabra *true* para que se envíen los datos.

Para Matlab 2021: se puede utilizar la función `write` para enviar el mensaje.

Para Matlab 2019: se utiliza la función `fprintf` para el envío de la cadena. 

En Matlab 2021: se pued0ne utilizar las funciones `readline y disp` para la recepción y despliegue de la cadena.

En Matlab 2019: se debe utilizar la función `fscanf` para la recepción de datos. 
