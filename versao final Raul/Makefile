
socket_cliente.o: socket_cliente.c
	gcc -pthread -o socket_cliente.o -c socket_cliente.c

ListaDinEncad.o: ListaDinEncad.c ListaDinEncad.h
	gcc -o ListaDinEncad.o -c ListaDinEncad.c

calendario.o: calendario.c calendario.h ListaDinEncad.c
	gcc -o calendario.o -c calendario.c

socket_servidor.o: socket_servidor.c calendario.c
	gcc -pthread -o socket_servidor.o -c socket_servidor.c

cliente: socket_cliente.o
	gcc -pthread -o cliente socket_cliente.o

servidor: socket_servidor.o calendario.o ListaDinEncad.o
	gcc -pthread -o servidor socket_servidor.o calendario.o ListaDinEncad.o

all: cliente servidor

run_client: cliente
	./cliente

run_server: servidor
	./servidor


clean: all
	rm -rf cliente servidor *.o

zip: clean
	zip -r trab_1.zip ./*
