# Robotic Operating System (ROS)
* Instalación 
	* Requerimientos
	* Instalación de Ubuntu en Microsoft 
	* Instalación de ROS
* Carpetas
	* Espacios de trabajo (Workspace)
	* Documentación 

## Instalación 
A continuación se describen los pasos a seguir para la instalación de ROS dentro de un entorno operativo de Windows.

### Requerimientos
* Ubuntu 20.04 (Focal Fossa)
	* procesador 2 GHz dual-core
	* 4 GB RAM
	* espacio en disco duro de 25 GB
	* cable VGA capaz de soportar resolución de 1024×768
	* Windows 10 Version 1903, Build 18362 o superior
* Gazebo
	* 	Una memoria GPU dedicada
	* 	Una CPU intel I5 o equivalente en rendimiento
	* 	500 Mb de espacio en memoria 
	* 	Ubuntu Trusty o versiones posteriores

### Instalación de Ubuntu en Microsoft 
Para poder correr aplicaciones de Linux directamente en windows podemos utilizar *Windows Subsystem for Linux*  [WSL2](https://docs.microsoft.com/en-us/windows/wsl/compare-versions) y la guía de instalación se puede encontrar en el siguiente[Enlace](https://docs.microsoft.com/en-us/windows/wsl/install). La distro de Linux que se utilizará es la mencionada anteriormente la cual es **ubuntu 20.04 (focal fossa)**.

### Instalación de ROS
Establecer las fuentes de ROS para obtener los datos:
```
sudo sh -c 'echo "deb http://packages.ros.org/ros/ubuntu $(lsb_release -sc) main" > /etc/apt/sources.list.d/ros-latest.list'
```

Instalar curl de ser necesario con el siguiente comando:
```
Sudo apt install curl
```

Verificar que fue instalado:
```
Curl — version
```

Establecer las claves para la instalación de ROS:
```
curl -s https://raw.githubusercontent.com/ros/rosdistro/master/ros.asc | sudo apt-key add -
```

Asegurarse que todos los paquetes estén actualizados:
```
Sudo apt update
```

Instalación completa de ROS noetic distro:
```
sudo apt install ros-noetic-desktop-full
```

Para establecer el ambiente de trabajo se debe de ejecutar el siguiente comando en cada línea de comando nueva:
```
source /opt/ros/noetic/setup.bash
```

O se puede agregar directamente a las fuentes de ubuntu con los siguientes comandos, así no es necesario ejecutarlo cada vez:
```
echo "source /opt/ros/noetic/setup.bash" >> ~/.bashrc
source ~/.bashrc
```

Se instalan las dependencias para compilar los paquetes de ROS:
```
sudo apt install python3-rosdep python3-rosinstall python3-rosinstall-generator python3-wstool build-essential
Sudo apt install python3-rosdep
```

Y luego de inicializa rosdep que es una herramienta que nos ayuda a instalar dependencias para compilar los paquetes y también es necesario para correr algunos elementos de ROS:
```
Sudo rosdep init
rosdep update
```

##  Carpetas
### Espacios de trabajo
Dentro de esta carpeta se encuentran los espacios en donde se realizan todas las simulaciones de ROS. Los archivos no pueden moverse aunque si pueden ser modificados. 
### Documentación 
Dentro de esta carpeta se encuentra toda la documentación de ROS utilizada para el desarrollo de la tesis.