
ClienteSocket.o: ClienteSocket.c
	gcc -o ClienteSocket.o -c ClienteSocket.c

trab_1: ClienteSocket.o
	gcc -o trab_1 ClienteSocket.o

all: trab_1

clean: all
	rm -rf trab_1 *.o

run: trab_1
	./trab_1

zip: clean
	zip -r trab_1.zip ./*
