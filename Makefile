# set compiler which is gcc and debug options as compiler options
CC=gcc
CFLAGS=-g

all:
	# we don't use that for now
# compile simply test to ./test and then run it
test:
	$(CC) $(CFLAGS) test.c -o test
	./test
