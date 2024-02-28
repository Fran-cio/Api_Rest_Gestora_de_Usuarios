# Sistemas operativos 2-Trabajo practico 6
## Informe
### Descripcion
Partiendo del tp anterior se usan los mismos conceptos, se coloca las nuevas rutas y los metodos que corresponde, eso carece de la mistica como tal.
### Make
Se crearon las instancias **install** y **remove**, tal como un setup y un uninstall.
Ejecutando cada uno obtenemos los efectos inversos, claramente.
El instalador copia los binarios a */usr/bin/*, carga el ngix con la configuracion en */etc/nginx/sites-enabled/* y les levanta el servicio, carga los 3 servicios generados y les da arranque, finalmente carga las configuraciones de logrotate en */etc/logrotate.d/*.
### Ngix
El objetivo de usar ngix era hacer que funcione como un proxy inverso que al recibir la request que sale de nuestro local host la redirija al path correspondiente con el binario.
Para eso se agregaron 2 elementos a la configuraciones del ngix.
Para crear las credenciales se uso ```  sudo htpasswd -c /etc/httpd/.htpasswd admin ```
```
    server {
            listen 80;
  
            server_name tp6.com.ar;
  
            auth_basic           "Administrator’s Area";
            auth_basic_user_file /etc/httpd/.htpasswd;
     
            location /{
                    auth_basic off;
                    return 404 '{"error" : {"code" : 404, "descripcion" : "Not found"}}';
            }
 
            location = /api/users {
                    proxy_pass http://localhost:7777/api/users;
            }
 
    }
 
    server {
            listen 80;
 
            auth_basic           "Administrator’s Area";
            auth_basic_user_file /etc/httpd/.htpasswd;
 
            server_name contadordeusuarios.com.ar;
 
            location / {
                    auth_basic off;
                    return 404 '{"error" : {"code" : 404, "descripcion" : "Not found"}}';
            }
 
            location /contador/increment{
                    proxy_pass http://localhost:6666/contador/increment;
            }
 
            location /contador/value {
                    proxy_pass http://localhost:6666/contador/value;
            }
    }
```
- *server_name* determina desde donde se va a redirigir, es decir, si yo le hago un curl a eso, lo voy a dirigir a las location de abajo
- En cada *location* se especifica cada path de la redireccion se redirija a otra del proxy
- *proxy_set_header* coloca un encabezado con la ip del remoto *(Esto se hace para una funcion del programa)*

### LogRotate
Con el install se copia la config en el dir logrotate.d, con un par de comandos obtenidos de:
[Administrar archivos de log con logrotate](https://styde.net/administrar-archivos-de-logs-con-logrotate/)

```
/var/log/tp6/*log{
    hourly
    missingok
    rotate 5
    compress
    create
}
```

### SystemD
Ya habia estado trabajando con el SystemD asi que ya venia con practica.
Se crearon 3 archivos, 2 servicios independientes y uno que los levanta juntos. En la instalacion se copian los servicios y se le da *start*. 

### api
Para destacar solamente queda decir que:
- Se agrego una acreditacion para la creacion de usuario, unicamente para el.
- La encriptacion se hace previa al envio al server, mediante el *.sh*.
- Se agregan distintos corta fuego en caso de algo salir mal y se notifica al cliente.
- El user ID se asiga al azar asegurandose de no repetir.

