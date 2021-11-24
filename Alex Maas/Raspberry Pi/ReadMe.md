Para este trabajo se uso el ordenador Raspberry Pi, al trabajar con un algoritmo de robótica de enjambre es indispensable que se cuente con al menos 4. Todas las pruebas realizadas en esta tesis fueron hechas con los modelos 3B, 3B+ y 4, el departamento cuenta con modelos 3B y 3B+ por lo que se recomienda contactar con ellos para pedirlas prestadas. 

Si se desea usar una propia se deberá instalar el siguiente software [Raspberry Pi Imager](http:www.raspberrypi.org/software/ "Raspberry Pi Imager") para quemar la imagen a su SD (se recomienda que sea de generación 10). Para más detalle o instalación de Visual Studio Code (usado en esta tesis) se recomienda ver guia "IE3027-lec6_Raspberry_Pi" desarrollada por Pablo Mazariego.



## Comando útiles

Para compilar algún código desde la terminar
```sh
gcc nombre_del_archivo.c -o _ejecutable_
```

Para compilar la librería  math agregar el comando 
```sh
-lm
```

Para compilar algún programa con multihilos agregar el comando 
```sh
-lpthread
```

Para apagar la Raspberry desde la terminal
```sh
sudo shutdown -h now
```

Para verificar si algún socket se encuentra abierto y forzar su terminación
```sh
ps ax | grep _ejecutable_
kill _num.ejecutable_
```

Para enviar algún archivo (o ejecutable) de una raspberry a otra (deben estar en la misma red)
```sh
scp _archivo_ pi@10.0.0.0:Desktop
```
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; <sub>Nota: modificar la direccion IP y la ubicacion de destino (ejemplo: Desktop).</sub>

Para conectarse desde la aplicación Cygwin (terminal) a alguna Raspberry 
```sh
ssh pi@10.0.0.0
```
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; <sub>Nota: modificar la direccion IP.</sub>