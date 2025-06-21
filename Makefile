# set compiler which is gcc and debug options as compiler options
CC=gcc
CFLAGS=-g


# compile simply test to ./test and then run it
test: 
	$(CC) $(CFLAGS) test.c -o test
	./test
run:
	./test
