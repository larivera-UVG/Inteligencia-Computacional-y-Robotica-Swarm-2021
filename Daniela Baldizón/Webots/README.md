## Webots
En esta carpeta se encuentran los archivos utilizados para la validación de las trayectorias en Webots. Las carpetas se detallan a continuación:

![MapaWebots](https://user-images.githubusercontent.com/45132555/143382042-70ed7599-cc68-4e77-a3d9-22683fe76a53.JPG)

### PYE
En esta carpeta se encuentra la información de la trayectoria generada por el algoritmo de planificación de trayectorias y evasión de obstáculos. 

### Resultados
En esta carpeta se encuentran los resultados generados por cada controlador. Hay una carpeta correspondiente para cada controlador. Los archivos Graficas.m toman los datos generados por Webots y devuelven las gráficas de las velocidades del robot e-puck. 

### Controllers
En esta sección se encuentra el controlador empleado para la validación de las trayectorias. El nombre de dicho controlador es PyE. Es importante que en el algoritmo de planificación de trayectorias y evasión de obstáculos estén descomentadas las siguientes líneas:

```
bpath = [G.Nodes.X(best_path), G.Nodes.Y(best_path)];
webots_path = (bpath - (grid_size/2+0.5)).*[1/5 -1/5];
wb_pc_path = 'C:\Users\Daniela Baldizon\Documents\GitHub\Inteligencia-Computacional-y-Robotica-Swarm-2021\Daniela Baldizón\Webots\controllers\PyE\';
save(strcat(wb_pc_path, 'webots_test.mat'), 'bpath', 'webots_path')

```

En mi caso le coloqué esa ruta al `wb_pc_path` pero esa se tiene que cambiar al correr el código. 

Para cambiar el controlador se debe modificar la variable `controlador`.

### Worlds
Aquí se encuentra el mundo de Webots en el que se realizaron las pruebas. Este es un archivo .wbt y este fue generado con Webots versión 2021a. Para ejectuar, solo se debe presionar la flecha de play en Webots. 
