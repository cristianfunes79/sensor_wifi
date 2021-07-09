# sensor_wifi
Repo para el firmware del sensor wifi

# funciones basicas
1- Conectarse a un SSID y PASS al encender  
    - checkear conectividad cada cierto tiempo  
2- Usar blink de un led para indicar status  
3- Conectar con el sensor, checkear que este conectado  
4- Realizar mediciones de temperatura  
5- Deteccion de corte de energia  
6- Medir tension de la bateria  
7- Armar un paquete ( determinar formato, por ej /temp:<temp>/bat:<bat>/!, /temp:23.32/bat:3.8/! ) y enviar por tcp ( o udp, ver que conviene )  
8- Implementar base de datos para almacenar historicos  

# funciones mas completas
1- Cuando se enciende puede arrancar un web server para que el usuario se conecte y le configure el ssid y password.  
  - Tambien se puede hacer un menu de configuracion mas completo para configurar cada sensor con una app  
2- Agregar un display oled para poder ver mas informacion comodamente, como la temperatura actual, ip, bateria.  
3- Ver si conviene enviar todos los datos o solo enviar un heartbeat que indique conexion, y enviar alarmas. Eso para ahorrar consumo.  
  - Con esto podriamos ver si logramos un consumo lo suficientemente bajo para usar pilas sin tener conectado a la red.  
  - En ese caso hay que crear un sensor que detecte corte de energia  
4- El tiempo de medicion puede ser distinto al de envio de datos.  
5- Determinar bien el formato de la trama.   
6- Enviar parametros de configuracion desde el equipo central hacia los sensores, por ejemplo tiempo de envio de datos.  
  - Tambien si se envia un nivel de bateria muy bajo se le puede mandar un comando para que se apague el sensor ( deberia apagarse pero volver al rato )  
7- Implementar algun protocolo como ping para ver que el sensor este "vivo"  


# funciones muy avanzadas    
1- Upgrade de fw por wifi  

