### Lab6 Sistemas Operativos II
## Ingeniería en Compuatación - FCEFyN - UNC
# Sistemas Embebidos

## Introducción
Los _sistemas embebidos_ suelen ser accedidos de manera remota. Existen distintas técnicas para hacerlo, una forma muy utilizada suelen ser las _RESTful APIs_. Estas, brindan una interfaz definida y robusta para la comunicación y manipulación del _sistema embebido_ de manera remota. Definidas para un esquema _Cliente-Servidor_ se utilizan en todas las verticales de la industria tecnológica, desde aplicaciones de _IoT_ hasta juegos multijugador.

## Objetivo
El objetivo del presente trabajo práctico es que el estudiante tenga un visión _end to end_ de una implementación básica de una _RESTful API_ sobre un _sistema embedido_.
El estudiante deberá implementarlo interactuando con todas las capas del procesos. Desde el _testing_ funcional (alto nivel) hasta el código en C del servicio (bajo nivel).

## Desarrollo
### Requerimientos
Para realizar el presente trabajo practico, es necesario una computadora con _kernel_ GNU/Linux, ya que usaremos [SystemD][sysD] para implementar el manejo de nuestro servicios.

### Desarrollo
Se deberá implementar dos servicios en lenguaje C, estos son el _servicio de usuarios_ y el _servicio contador_. Cada servicio deberá exponer una _REST API_ con _Media Type_ `application/json` para todas sus funcionalidades. Con el objetivo de acelerar el proceso de desarrollo vamos a utilizar un _framework_: se propone utilizar [ulfius][ulfi]. El estudiante puede seleccionar otro, justificando la selección, o implementar el propio (no recomendado).
El servicio debe tener configurado un [nginx][ngnx] por delante para poder direccionar el _request_ al servicio correspondiente.
El web server, deberá autenticar el _request_ por medio de un usuario y password enviado en el _request_, definido donde el estudiante crea conveniente. Las credenciales no deberán ser enviadas a los servicios. 


El web server deberá *solo* responder a `lab3.com` para el servicio de usuarios y `contadordeusuarios.com` para el servicio de contador. Debe retornar _404 Not Found_ para cualquier otro _path_ no existente con algún mensaje a elección con formato JSON.

A modo de simplificación, usaremos sólo _HTTP_, pero aclarando que esto posee *graves problemas de seguridad*.
Todos los servicios deben estar configurados con _SystemD_ para soportar los comandos, _restart_, _reload_, _stop_, _start_ y deberán ser inicializados de manera automática cuando el sistema operativo _botee_.



Algunos servicios requieran _logear_ todas sus peticiones con el siguiente formato:

```sh
    <Timestamp> | <Nombre Del Servicio> | <Mensaje>
```

El _\<Mensaje\>_ será definido por cada una de las acciones de los servicios.

El gráfico \ref{fig:arq} se describe la arquitectura requerida.

Se debe implementar un mecanismo para [rotar los logs][logrotate]

![Arquitectura del sistema](Web_App.png)


A continuación, detallaremos los dos servicios a crear y las funcionalidades de cada uno. 

### Servicio de Usuarios
Este servicio se encargará de crear usuarios y listarlos. Estos usuarios deberán poder _logearse_ vía _SSH_ luego de su creación.

#### POST /api/users
Endpoints para la creación de usuario en el sistema operativo:

```C
    POST http://{{server}}/api/users
```
Request
```C    
        curl --request POST \
            --url http:// {server}}/api/users \
            -u USER:SECRET \
            --header 'accept: application/json' \
            --header 'content-type: application/json' \
            --data '{"username": "myuser", "password": "mypassword"}'
```
Respuesta
```C

        {
            "id": 142,
            "username": "myuser",
            "created_at": "2019-06-22 02:19:59"
        }

```
El _\<Mensaje\>_ para el log será: _Usuario \<Id\> creado_.

Cada vez que se cree un usuario, este servicio deberá incrementar el contador en el servicio de contador de usuarios. 
En caso que el servicio de contador no este diposnible o falle, logear al falla.
  
#### GET /api/users
Endpoint para obtener todos los usuarios del sistema operativo y sus identificadores.
```C
    GET http://{{server}}/api/users
```
Request
```C
    curl --request GET \
        --url http://{{server}}/api/users \
        -u USER:SECRET \
        --header 'accept: application/json' \
        --header 'content-type: application/json'
```
Respuesta
```C
    {
      "data": [
          {
              "user_id": 2,
              "username": "user1",  
          },
          {
              "user_id": 1,
              "username": "user2"
          },
          ...
      ]
    }
```
El  _\<Mensaje\>_ para el log será:  _Usuario creados: \<cantidad de usuario del SO\>_
 
### Servicio de contador
Este microservicio pose los endpoint del laboratorio 5, lleva un contador. Este contador debe ser implementado con el mismo _Media Type_ , `application/json`.

#### POST /contador/increment
```C
    POST http://{{server}}/contador/increment
```
Request

```C
    curl --request POST \
        --url http://{{server}}/contador/increment \
        -u USER:SECRET \
        --header 'accept: application/json' \
        --header 'content-type: application/json'
```

Respuesta
```C
    {
        "code": 200,
        "description": "<new_value>"
    }
```

El  _\<Mensaje\>_ para el log será:  _Contador Incrementado desde: \<El ip del cliente que incremento el contador.\>_


#### GET /contador/value
Este endpoint permite saber el valor actual del contador
```C
    GET http://{{server}}/contador/value
```
Request
```C
    curl --request GET \
        --url http://{{server}}/contador/value \
        -u USER:SECRET \
        --header 'accept: application/json' \
        --header 'content-type: application/json'
```
Respuesta

```C
    {
        "code": 200,
        "description": "<value>"
    }
```


Este endpoint no tiene ningún requerimiento de para logging.
 
## Entrega
Se deberá proveer los archivos fuente, así como cualquier otro archivo asociado a la compilación, archivos de proyecto "Makefile" y el código correctamente documentado, todo en el repositorio, donde le Estudiante debe demostrar avances semana a semana mediante _commits_.

También se debe entregar un informe, guia tipo _How to_, explicando paso a paso lo realizado (puede ser un _Markdown_). El informe además debe contener el diseño de la solución con una explicacion detallada de la misma. Se debe asumir que las pruebas de compilación se realizarán en un equipo que cuenta con las herramientas típicas de consola para el desarrollo de programas (Ejemplo: gcc, make), y NO se cuenta con herramientas "GUI" para la compilación de los mismos (Ej: eclipse).

El install del makefile deberá copiar los archivos de configuración de systemd para poder luego ser habilitados y ejecutados por linea de comando.
El script debe copiar los archivos necesarios para el servicio Nginx systemd para poder luego ser habilitados y ejecutados por linea de comando.
Los servicios deberán pasar una batería de test escritas en _postman_ provistas. TBD.

### Criterios de Corrección
- Se debe compilar el código con los flags de compilación: 
     -Wall -Pedantic -Werror -Wextra -Wconversion -std=gnu11
- La correcta gestion de memoria.
- Dividir el código en módulos de manera juiciosa.
- Estilo de código.
- Manejo de errores
- El código no debe contener errores, ni warnings.
- El código no debe contener errores de cppcheck.


## Evaluación
El presente trabajo práctico es individual deberá entregarse antes del jueves 27 de Mayo de 2022 a las 23:55 mediante el LEV.  Será corregido y luego deberá coordinar una fecha para la defensa oral del mismo.

## Referencias y ayudas
- [Systrem D ](https://systemd.io/)
- [System D en Freedesktop](https://www.freedesktop.org/wiki/Software/systemd/)
- [nginx](https://docs.nginx.com/)
- [Ulfius HTTP Framework](https://github.com/babelouest/ulfius)
- [Kore Web PLataform](https://kore.io/)

[sysD]: https://www.freedesktop.org/wiki/Software/systemd/
[ngnx]: https://docs.nginx.com/
[ulfi]: https://github.com/babelouest/ulfius
[logrotate]: https://en.wikipedia.org/wiki/Log_rotation
