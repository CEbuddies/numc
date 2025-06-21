# set compiler which is gcc and debug options as compiler options
CC=gcc
CFLAGS=-g

all: main.c NumC.h
	$(CC) $(CFLAGS) main.c -o main
	./main
test: 
	$(CC) $(CFLAGS) test.c -o test
	./test
run:
	./test
