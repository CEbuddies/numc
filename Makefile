all:
	gcc -c libNumC.c
	gcc -c main.c
	gcc -o test libNumC.o main.o
run:
	./test
