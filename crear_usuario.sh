#!/bin/bash
echo "ingrese credenciales:"
echo -n "Usuario: "
read -r usuario
echo -n "Contrasenia: "
read -r contrasenia
echo -n "Nombre de usuario nuevo: "
read -r new_user
echo -n "contrasenia nueva: "
read -r new_pass
new_password=$(perl -e 'print crypt($ARGV[0], "password")' $new_pass)
json="{\"username\": \"$new_user\", \"password\": \"$new_password\"}"
echo -n $json | http -v POST tp6.com.ar/api/users --auth $usuario:$usuario
