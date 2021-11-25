[![Build Status](https://travis-ci.org/joemccann/dillinger.svg?branch=master)](https://travis-ci.org/joemccann/dillinger)

# Aplicaciones Prácticas para Algoritmos de Inteligencia y Robótica de Enjambre
En este proyecto se emplea el algoritmo _Ant Colony Optimization_ (ACO) en Matlab para implementar aplicaciones de inteligencia y robótica de enjambre. Los algoritmos implementados son los siguientes:

- Algoritmo de planificación de trayectorias y evasión de obstáculos. 
- Algoritmo de exploración de terrenos

El algoritmo de planificación de trayectorias y evasión de obstáculos se utilizó como planificador de trayectorias para el robot E-Puck. Se validaron dichas trayectorias haciendo uso de Webots.

![539e8bc7d602c0e65dd80bda08234695](https://user-images.githubusercontent.com/45132555/143372513-61c23b55-f1f2-4a5f-a2ef-7b715c77103d.gif)

## Tabla de contenido

1. [Prerrequisitos](#desc)
2. [ Carpetas ](#usage)
   1. [Documentos](#docs)
   2. [Exploración de terrenos](#exp)
   3. [Fase anterior](#fant)
   4. [Planificación y evasión](#pye)
   5. [Registro de imágenes](#regis)
   6. [Webots](#webots)

<a name="desc"></a>
## Prerrequisitos
Para correr los programas es necesario tener instalado Matlab. Para la elaboración del código se utilizó la versión 2020a y para realizar algunas pruebas se utilizó la versión 2021a. En el caso de la validación de las trayectorias, se utilzó Webots versión 2021a. Los controladores se implementaron en Matlab, por lo que es necesario realizar la conexión entre Matlab y Webots. Para la conexión visitar el siguiente link: [Matlab+Webots].

**Asegurarse que el ejecutable de Matlab está en carpetas que se llaman Matlab. El path debería verse de la siguiente forma:**
`C:\Program Files\MATLAB\R2020a\bin`

**Esto es muy importante para que funcione Matlab con Webots.**

<a name="usage"></a>
## Carpetas
A continuación se presenta un resumen de lo que contienen las carpetas dentro de este repositorio. 

<a name="docs"></a>
### Documentos
En esta carpeta se encuentran las entregas del protocolo y de la tesis. También se encuentran algunas figuras utilizadas en el documento de tesis, los artículos encontrados durante el proceso de investigación y los bibtex de las referencias. 

<a name="exp"></a>
### Exploración de terrenos
En esta carpeta se encuentra el código de esta implementación y los resultados obtenidos durante las pruebas.
![iyiy](https://user-images.githubusercontent.com/45132555/143372250-ec256dae-17bc-4ab2-bc3b-580b1428ddd1.png)

<a name="fant"></a>
### Fase anterior
Aquí está el trabajo realizado en el trabajo previo a este. Estos códigos se tomaron como base para las implementaciones.

<a name="pye"></a>
### Planificación y evasión
En esta carpeta está el código de la implementación del algoritmo de planificación de trayectorias y evasión de obstáculos. Asimismo, se incluye la carpeta con los resultados obtenidos de las pruebas y de los barridos de parámetros. 

<a name="regis"></a>
### Registro de imágenes
En esta carpeta se encuentr el código de la implementación del algoritmo de registro de imágenes. Asimisno, se encuentran las funciones necesarias para el funcionamiento correcto del algoritmo y los resultados obtenidos durante la experimentación. 

![5d63cfc871ee2151a4daac45b6f84d6c](https://user-images.githubusercontent.com/45132555/143372367-7c543be0-aca8-470a-b6a6-40c80808f3fa.gif)

<a name="webots"></a>
### Webots
En esta sección se encuentran los archivos utilizados en Webots para la validación de las trayectorias. Se incluyen los archivos de matlab con los controladores y los resutlados obtenidos. 

[Matlab+Webots]: <https://cyberbotics.com/doc/guide/using-matlab>
