[![Build Status](https://travis-ci.org/joemccann/dillinger.svg?branch=master)](https://travis-ci.org/joemccann/dillinger)

# Aplicaciones Prácticas para Algoritmos de Inteligencia y Robótica de Enjambre
En este proyecto se emplea el algoritmo _Ant Colony Optimization_ (ACO) en Matlab para implementar aplicaciones de inteligencia y robótica de enjambre. Los algoritmos implementados son los siguientes:

- Algoritmo de planificación de trayectorias y evasión de obstáculos. 
- Algoritmo de exploración de terrenos

El algoritmo de planificación de trayectorias y evasión de obstáculos se utilizó como planificador de trayectorias para el robot E-Puck. Se validaron dichas trayectorias haciendo uso de Webots.

## Tabla de contenido

1. [Prerrequisitos](#desc)
2. [ Uso del código ](#usage)
   1. [ Documentos ](#docs)
   2. [Exploración de terrenos](#exp)
   3. [Fase anterior](#fant)
   4. [Planificación y evasión](#pye)
   5. [Registro de imágenes](#regis)
   6. [Webots](#webots)

<a name="desc"></a>
## Prerrequisitos
Para correr los programas es necesario tener instalado Matlab. Para la elaboración del código se utilizó la versión 2020a y para realizar algunas pruebas se utilizó la versión 2021a. En el caso de la validación de las trayectorias, se utilzó Webots versión 2021a. Los controladores se implementaron en Matlab, por lo que es necesario realizar la conexión entre Matlab y Webots. Para la conexión visitar el siguiente link: [Matlab+Webots].

<a name="usage"></a>
## Uso del código 
A continuación se presenta un resumen de lo que contienen las carpetas dentro de este repositorio. 

<a name="docs"></a>
### Documentos
En esta carpeta se encuentran las entregas del protocolo y de la tesis. También se encuentran algunas figuras utilizadas en el documento de tesis, los artículos encontrados durante el proceso de investigación y los bibtex de las referencias. 

<a name="exp"></a>
### Exploración de terrenos
En esta carpeta se encuentra el código de esta implementación y los resultados obtenidos durante las pruebas.

<a name="fant"></a>
### Fase anterior
Aquí está el trabajo realizado en el trabajo previo a este. Estos códigos se tomaron como base para las implementaciones.

<a name="pye"></a>
### Planificación y evasión
En esta carpeta está el código de la implementación del algoritmo de planificación de trayectorias y evasión de obstáculos. Asimismo, se incluye la carpeta con los resultados obtenidos de las pruebas y de los barridos de parámetros. 

<a name="regis"></a>
### Registro de imágenes

<a name="webots"></a>
### Webots
En esta sección se encuentran los archivos utilizados en Webots para la validación de las trayectorias. Se incluyen los archivos de matlab con los controladores y los resutlados obtenidos. 

[Matlab+Webots]: <https://cyberbotics.com/doc/guide/using-matlab>
