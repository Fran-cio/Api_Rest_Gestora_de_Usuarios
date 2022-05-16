CC=gcc
GDB=-g
CFLAGS=-Wall -pedantic -Werror -Wextra -Wconversion -std=gnu11
PATHbin=./bin/
PATHrec=./src/
PATHout=./out/
PATHlib=./lib/
LAB6LIB=-lulfius -lorcania -ljansson -lyder -L./ulfius/src/

lab6: $(PATHbin)api_gestor_usuario $(PATHbin)api_contador

$(PATHbin)api_contador: $(PATHrec)api_contador.c 
	mkdir -p $(PATHbin) 
	$(CC) $(CFLAGS) -o $(PATHbin)api_contador $(PATHrec)api_contador.c $(LAB6LIB)

$(PATHbin)api_gestor_usuario: $(PATHrec)api_gestor_usuario.c
	mkdir -p $(PATHbin) 
	$(CC) $(CFLAGS) -o $(PATHbin)api_gestor_usuario $(PATHrec)api_gestor_usuario.c $(LAB6LIB)
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
