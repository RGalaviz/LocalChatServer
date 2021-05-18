# Actividad colaborativa - Manejo de sockets
Escribe un programa Cliente-Servidor que envía un mensaje de un byte del cliente al servidor, y luego haga que el servido devuelva el mensaje al cliente. Este proceso debe repetirse 1000 veces, y debe calcularse la latencia de ida y vuelta promedio.

Este proceso debe seguirse también para mensaje de tamaño 1, 2, 4, 8 y 16 KB. El cliente despliega en pantalla una tabla con la latencia promedio para cada paso.

```
$ .\server ip port
```

```
$ .\client ip port
```

|     | Ponderación                                                                                                                                                                                                           |
|-----|-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| +5  | El servidor verifica que recibe la cantidad correcta de parámetros.<br>En caso de que no sea así, el programa despliega un mensaje <br>adecuado y termina, regresando -2 como resultado de su ejecución.              |
| +5  | El servidor verifica que *port* sea un número entero valido (mayor <br>a 5000). En caso de que no sea así, el programa despliega un <br>mensaje adecuado y termina, regresando -3 como resultado de su <br>ejecución. |
| +5  | El cliente verifica que recibe la cantidad correcta de parámetros. <br>En caso de que no sea así, el programa despliega un mensaje <br>adecuado y termina, regresando -2 como resultado de su ejecución.              |
| +5  | El cliente verifica que puerto sea un número entero valido (mayor <br>a 5000). En caso de que no sea así, el programa despliega un <br>mensaje adecuado y termina, regresando -3 como resultado de su <br>ejecución.  |
| +30 | El servidor espera por una única conexión, se realizan las pruebas <br>y cierra adecuadamente todas las conexiones.                                                                                                   |
| +30 | El cliente se conecta, realizas las pruebas en la forma definida <br>en el problema, despliega los resultados en forma tabular y cierras <br>adecuadamente la conexión.                                               |
| +10 | Los resultados se despliegan de manera legible.                                                                                                                                                                       |