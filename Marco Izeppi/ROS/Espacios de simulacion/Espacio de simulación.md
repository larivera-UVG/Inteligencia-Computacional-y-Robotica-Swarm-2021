# Espacio de simulación 
Dirigirse a esta carpeta dentro del cmd de Linux o el cmd de Linux en Windows. Ya estando dentro de esta carpeta se debe de utilizar una línea de comandos para correr el siguiente comando:
```
roscore
```
Este comando inicializa la comunicación de los nodos, la conexión entre todos los nodos y todo lo relacionado al entorno de ROS.

para correr simulaciones se pueden ver todos los archivos .launch que se encuentran en el folder de “launch”. Para que al algoritmo de dijkstra funcione, se debe de correr el entorno de simulación de gazebo primero, eso de hace con el siguiente comando:
```
roslaunch unit_2pp simulation.launch
```
Luego de correr el anterior comando se puede inicializar el algoritmo de dijkstra utilizando el siguiente comando:
```
roslaunch unit2_pp Dijkstra.launch
```
Esto automáticamente abrirá la aplicación de Ruiz. Para visualizar el funcionamiento del algoritmo, en la parte superior se encuentra un botón que dice “2D Goal” como se observa en la figura a continuación:


Luego de presionar el botón, se puede seleccionar cualquier punto dentro de la cuadrícula y se puede ver como el algoritmo busca a través de todos los nodos dentro del mapa y determina la mejor ruta desde la posición del robot a la posición seleccionada.