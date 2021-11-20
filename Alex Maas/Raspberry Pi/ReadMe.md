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