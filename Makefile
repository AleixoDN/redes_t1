
ClienteSocket.o: ClienteSocket.c
	gcc -o ClienteSocket.o -c ClienteSocket.c

ServidorSocket.o: ServidorSocket.c
	gcc -o ServidorSocket.o -c ServidorSocket.c

cliente: ClienteSocket.o
	gcc -o cliente ClienteSocket.o

servidor: ServidorSocket.o
	gcc -o servidor ServidorSocket.o

all: cliente servidor






socket_cliente.o: socket_cliente.c
	gcc -pthread -o socket_cliente.o -c socket_cliente.c

socket_servidor.o: socket_servidor.c
	gcc -pthread -o socket_servidor.o -c socket_servidor.c

cliente2: socket_cliente.o
	gcc -pthread -o cliente2 socket_cliente.o

servidor2: socket_servidor.o
	gcc -pthread -o servidor2 socket_servidor.o

all2: cliente2 servidor2

run_c2: cliente2
	./cliente2

run_s2: servidor2
	./servidor2






clean: all
	rm -rf cliente servidor *.o

run_c: cliente
	./cliente

run_s: servidor
	./servidor

zip: clean
	zip -r trab_1.zip ./*
