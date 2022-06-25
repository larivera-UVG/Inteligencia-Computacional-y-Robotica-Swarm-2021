# Diseño e Implementación de una Plataforma Móvil ParaAplicaciones de Robótica de Enjambre - Fase III

Este trabajo de graduación representa la tercera fase de la plataforma desarrollada para aplicaciones en robótica de enajambre del año 2018, en este trabajo se hace una nueva 
adaptación con respecto a los nuevos y pasados módulos utilizados en la fase anterior, siendo las nuevas adquisiciónes una cámara, y un cambio en el microcontrolador por una Raspberry Pi Zero W 

## Requsitos 📋

Los requisitos de software para implementar en Raspberry deben ser las librerías de la cámara, sockets para comunicación con la computadora. 

Las carpetas continen los siguientes resultados


### Diseño 3D 🔧

En este se encuentran todos los modelos, partes creadas y ensamblajes hechos para poder simularlo en Inventor 2022. Los ensambles son mayores a 25mb por lo que se debieron subir a Google Drive, pueden ingresar presionando 
[Aquí](https://drive.google.com/drive/folders/1uAZ89YliQH7GU5Ns9C6hzhxoIw6cb_pV?usp=sharing "Aqui")

### Documentos 

En esta sección se tienen los CAD dimensionados, presupuesto, archivos para la cortadora laser y documentos de tesis.
La carpeta Documentos para manufactura contiene los documentos necesarios para poder replicar la placa PCB o la base en mdf para futuros proyectos.

### Programación 

Se divide en 3 partes: 
-Codigos definitivos: codigos de los demos realizados utilizando todos los modulos y comunicación tcp y udp
-Codigos iniciales: distintos apoyos para cada uno de los modulos de la plataforma probado individualmente
-Comunicación inalambrica: distintos metodos para poder realizar una comunicación exitosa tanto en TCP como en UDP
(En el envio de datos, se incluye una versión para enviarlos en Python 3,8 que es más eficiente, esta no se llego a utilizar debido a que se trabajo todo el proyecto con python 3.7)
