all: customer.o metadata.o main.o
	clear
	gcc -o main main.o customer.o metadata.o -lm
	./main

test1: customer.o metadata.o main.o rm
	clear
	gcc -o main main.o customer.o metadata.o -lm
	./main 1

test2: customer.o metadata.o main.o rm
	clear
	gcc -o main main.o customer.o metadata.o -lm
	./main 2

test3: customer.o metadata.o main.o rm
	clear
	gcc -o main main.o customer.o metadata.o -lm
	./main 3

build: customer.o metadata.o main.o
	gcc -o main main.o customer.o metadata.o

main.o: main.c customer.h metadata.o
	gcc -o main.o main.c -c

customer.o: customer.c customer.h
	gcc -o customer.o customer.c -c

metadata.o: metadata_manager.c metadata_manager.h
	gcc -o metadata.o metadata_manager.c -c

rm: 
	echo -n > files/hash.dat
	echo -n > files/customers.dat
	echo -n > files/metadata.dat