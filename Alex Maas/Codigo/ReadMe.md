# Codigo
A continuación se dará una breve explicación de cada código incluido en esta carpeta, como ejecutarlo y como fue validado. 

## PSO.c

 
## pruebaEnvioyRecepcion_1Hilo.c
Este código evalúa la capacidad de enviar y recibir información de varios agentes por medio del protocolo de comunicación UDP tipo broadcast. Se introduce manualmente el costo local, global, numero de agente, posición en X, posición en Y y orientación de cada agente; estos son agrupados y enviados por medio del código principal y por medio de un hilo de programación son recibidos por todos los agentes. El hilo de recepción es capaz de identificar si se esta recibiendo datos de la pose de los agentes o datos del costo por posición de cada agente. 
Se debe verificar la dirección del broadcast  

## pruebaEnvioyRecepcion_2Hilo.c
Este código evalúa la capacidad de ejecutar un código principal y 2 hilos de programación simultáneos que estarán recibiendo información de forma constante. Se combina el Código desarrollado anteriormente "pruebaEnvioyRecepcion_1Hilo" con un nuevo hilo que estará recibiendo la pose de los agentes mediante un algoritmo de visión por computadora desarrollado como otro proyecto de tesis.

## pruebaPSO_1agente.c

## pruebaSTRTOK.c
En este código se prueban las funciones “strtok”, “atof” y “sprintf”. Se elabora un código al cual se le introduce una cadena de caracteres separados por comas, esta cadena es separada por la función strtok y por medio de la función atof cada elemento de la cadena es convertido a su valor numérico. Este proceso se continúa haciendo hasta haber terminado con el ultimo elemento de la cadena de caracteres iniciales. Finalmente, cada elemento se vuelve a agrupar dentro de otra variable en un orden especifico mediante la función sprintf. 

## prueba_comunicacionUDP_C&Matlab.c 
Para el envió de información entre cada agente y la mesa de pruebas de la UVG se estableció un protocolo de comunicación UDP, el cual se probó en esta versión de código. Este cosido funciona en conjunto con " prueba_comunicacionUDP_Matlab&C.m" y funciona como servidor, tiene como objetivo recibir y desplegar la información recibida. 

## prueba_comunicacionUDP_Matlab&C.m
Para el envió de información entre cada agente y la mesa de pruebas de la UVG se estableció un protocolo de comunicación UDP, el cual se probó en esta versión de código. Este cosido funciona en conjunto con " prueba_comunicacionUDP_C&Matlab.c " y funciona como cliente, tiene como objetivo agrupar y enviar la información a direcciones específicas.

## prueba_enviodecoordenadas_Webots.m
Este código tiene realiza la extracción y ordenamiento de la información proveniente de webots (archivo de texto), la cual contiene la trayectoria a seguir por los agentes. Posteriormente esta se envía a direcciones especificas en un intervalo de tiempo establecido por el usuario.

## PSO3.py
Esta versión del algoritmo PSO 2D consiste en una implementación ordinaria del conocido algoritmo para su comparación con el mismo algoritmo desarrollado en fases anteriores en Matlab. Esta implementación se realiza para Python, aprovechando sus múltiples librerías para graficar y su facilidad para trabajar con clases. Este código permite variar el numero de iteraciones, cantidad de agentes, seleccionar algún tipo de inercia y función costo, así como desplegar la iteración final en forma gráfica.  Se muestra la mejor posición alcanzada por los agentes según el máximo de iteraciones establecidas. Para tener una comparación justa con Matlab el espacio de búsqueda se limita a [-5,5] y se toman los agentes como partículas sin dimensiones físicas.