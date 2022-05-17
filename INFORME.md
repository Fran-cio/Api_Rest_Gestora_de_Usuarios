# Sistemas operativos 2-Trabajo practico 6
## Informe
### Descripcion
Partiendo del tp anterior se usan los mismos conceptos, se coloca las nuevas rutas y los metodos que corresponde, eso carece de la mistica como tal.
### Ngix
El objetivo de usar ngix era hacer que funcione como un proxy inverso que al recibir la request que sale de nuestro local host la redirija al path correspondiente con el binario.
Para eso se agregaron 2 elementos a la configuraciones del ngix.
```
    server {
	    listen 80;

	    server_name tp6.com.ar;

	    location /{
		    proxy_set_header X-Forwarded-For $remote_addr;
		    proxy_pass http://localhost:7777/;
	    }

	    location = /api/users {
		    proxy_set_header X-Forwarded-For $remote_addr;
		    proxy_pass http://localhost:7777/api/users;
	    }

    }

    server {
	    listen 80;

	    server_name contadordeusuarios.com.ar;

	    location / {
		    proxy_set_header X-Forwarded-For $remote_addr;
		    proxy_pass http://localhost:6666/;
	    }

	    location /contador/value {
		    proxy_set_header X-Forwarded-For $remote_addr;
		    proxy_pass http://localhost:6666/contador/value;
	    }
    }
}
```
- *server_name* determina desde donde se va a redirigir, es decir, si yo le hago un curl a eso, lo voy a dirigir a las location de abajo
- En cada *location* se especifica cada path de la redireccion se redirija a otra del proxy
- *proxy_set_header* coloca un encabezado con la ip del remoto *(Esto se hace para una funcion del programa)*

### LogRotate
Decidi hacerlo de manera local, solamente con un par de comandos obtenidos de:
[Administrar archivos de log con logrotate](https://styde.net/administrar-archivos-de-logs-con-logrotate/)

```
"./log_user.log"{
    hourly
    missingok
    rotate 5
    compress
    create
}
```
Con ese *config* y ```  ╱  ~  logrotate ./logrotate.conf --state ./logrotate-state --verbose ```.
Obtenemos un mensaje de **OK**.

### SystemD
Ya habia estado trabajando con el SystemD asi que ya venia con practica.
Generamos un servicio que tenga como tarea levantar los 2 binarios de nuestro programa.
Entonces una vez generado, le damos **start** con ```systemclt start``` y con ```system enable``` generamos la tarea de cada vez que el sistema se inicie.

### api
Para destacar solamente queda decir que:
- Se agrego una acreditacion para la creacion de usuario, unicamente para el.
- La encriptacion se hace previa al envio al server, mediante el *.sh*.
- Se agregan distintos corta fuego en caso de algo salir mal y se notifica al cliente.
- El user ID se asiga al azar asegurandose de no repetir.

