CC=gcc
GDB=-g
CFLAGS=-Wall -pedantic -Werror -Wextra -Wconversion -std=gnu11
PATHbin=./bin/
PATHrec=./src/
PATHout=./out/
PATHlib=./lib/
LAB6LIB=-lulfius -lorcania -ljansson -lyder -L./ulfius/src/

lab6: $(PATHbin)api_gestor_usuario $(PATHbin)api_contador

$(PATHbin)api_contador: $(PATHrec)api_contador.c $(PATHrec)funciones/funciones_contador.c 
	mkdir -p $(PATHbin) 
	$(CC) $(CFLAGS) -o $(PATHbin)api_contador $(PATHrec)api_contador.c $(LAB6LIB)

$(PATHbin)api_gestor_usuario: $(PATHrec)api_gestor_usuario.c $(PATHrec)funciones/funciones_user.c
	mkdir -p $(PATHbin) 
	$(CC) $(CFLAGS) -o $(PATHbin)api_gestor_usuario $(PATHrec)api_gestor_usuario.c $(LAB6LIB)

install:
	# Pre-requisitos:
	#	-ulfius
	#	-yder
	#	-janssen
	#	-ngix
	#-------- Bin  ---------
	make
	cp ./bin/api_contador /usr/bin/
	cp ./bin/api_gestor_usuario /usr/bin/
	#-------- Ngix ---------
	mkdir -p /etc/nginx/sites-enabled
	cp ./configs/tp6.conf /etc/nginx/sites-enabled
	mkdir -p /etc/httpd
	cp ./configs/htpasswd /etc/httpd/.htpasswd 
	systemctl stop nginx
	systemctl start nginx
	# Habilitar el include del sites-enabled en /etc/nginx/nginx.conf
	#-------- SystemD ------
	cp ./configs/*.service /etc/systemd/system/
	systemctl daemon-reload  
	systemctl start tp6
	systemctl enable tp6
	#-------- LogRotate ----
	mkdir -p /var/log/tp6
	cp ./configs/tp6_logrotate /etc/logrotate.d/
	#Exitoso

remove:
	# Pre-requisitos:
	#	-ulfius
	#	-yder
	#	-janssen
	#	-ngix
	#-------- Bin  ---------
	rm /usr/bin/api_contador
	rm /usr/bin/api_gestor_usuario
	#-------- Ngix ---------
	rm /etc/nginx/sites-enabled/tp6.conf
	systemctl stop nginx
	systemctl start nginx
	# Habilitar el include del sites-enabled en /etc/nginx/nginx.conf
	#-------- SystemD ------
	cp ./configs/*.service /etc/systemd/system/
	rm /etc/systemd/system/tp6*
	systemctl daemon-reload  
	systemctl stop tp6_user
	systemctl stop tp6_contador
	#-------- LogRotate ----
	rm -r -f /var/log/tp6/* /var/log/tp6
	rm /etc/logrotate.d/tp6_logrotate
	#Exitoso


#
#
# $(PATHlib)lib_rec.a: $(PATHout)recursos.o  
# 	mkdir -p $(PATHlib) 
# 	ar cr $(PATHlib)lib_rec.a $(PATHout)recursos.o 
#
# $(PATHout)recursos.o: $(PATHrec)recursos.c $(PATHrec)PGM_in_c.c
# 	mkdir -p $(PATHout)
# 	$(CC) $(CFLAGS) $(OPT) $(PARALELA) -c $(PATHrec)recursos.c 
# 	mv ./recursos.o $(PATHout)/recursos.o
#


clean:
	rm -f -d $(PATHbin)* $(PATHbin) $(PATHout)* $(PATHout) $(PATHlib)* $(PATHlib) 
