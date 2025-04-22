#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// TODO: check if we can generally typecast a struct

#ifndef NUMC_H
#define NUMC_H

typedef union {
    struct {
        int s0;
        int s1;
        int s2;
        int s3;
    } shapef;
    int sh[4];
} Shape;

// can be used to find a value for exmaple
typedef struct {
	size_t idx;
	double val;
} Tuple;

#define SHAPE(a, b, c, d) \
    ((Shape) {.sh[0]=a, .sh[1]=b, .sh[2]=c, .sh[3]=d})

typedef enum {
    INT,
    FLT,
    DBL
} Type;

// standard array with a void pointer
typedef struct {
    void * array;
    int len;
    Type type;
} Array;

typedef struct {
    int8_t * array;
    int len;
    Type type;
} ArrayI;

typedef struct {
    int8_t * array;
    int len;
    Type type;
} ArrayI8;

typedef struct {
	ArrayI (*zeros)(int len);
	void (*fill)(ArrayI array, int8_t val);
} Int;

typedef struct {
	ArrayI8 (*zeros)(int len);
	Tuple (*where)(ArrayI8 array, int val);
} Int8;

typedef struct {
    Array (*randint)(int len);
    ArrayI (*zeros)(int len); 
    int (*max)(Array array); // should take any array
    int (*min)(Array array);
    double (*scalar)(Array a1, Array a2);
    Int I;
    Int8 I8;
} NumC;


NumC numcinit();
/*
double std_scalar(Array a1, Array a2){
    // here goes checking
    // TODO: do proper error handling here
    if (a1.len != a2.len){
        printf("Trying to multiply arrays of different shapes\n");
        return -1;
    }
    double sum = 0;
    for (int i=0; i<a1.len; i++){
        sum += a1.array[i] * a2.array[i];
    }
    return sum;
}
*/
Array rint_(int len){
    int * array = calloc(len, sizeof(int));
    srand(time(NULL));
    int r;
    for (int i=0; i<len; i++){
        array[i] = rand();
    }
    // fill the array
    Array r_array;
    r_array.array = array;
    r_array.len = len;
    r_array.type = INT;
    
    return r_array;
}
ArrayI __zeros(int len){
	ArrayI array;
	array.len = len;
	array.type = INT;
	array.array = calloc(len, sizeof(int8_t));

	return array;
}

ArrayI8 __zerosi8(int len){
	ArrayI8 array;
	array.len = len;
	array.type = INT;
	array.array = calloc(len, sizeof(int8_t));
	return array;
}


void __fill(ArrayI array, int8_t val){
	for (int i=0; i<array.len; i++) {
		array.array[i] = val;
	}
}
double __isum(ArrayI array) {
	double sum = 0;
	for (int i=0; i<array.len; i++) {
		sum += array.array[i];
	}
	return sum;
}

Tuple __where(ArrayI8 array, int val) {
	Tuple t;
	for (int i=0; i<array.len; i++) {
		if (array.array[i] == val) {
			t.idx = i;
			t.val = array.array[i];
		}
	}
	return t;
}

// how to clear this later?
NumC numcinit(){

    NumC nc;
    // nc.max = &__maxint;
    // nc.min = &__minint;
    nc.zeros = &__zeros;
    nc.randint = &rint_;
    nc.I.zeros = &__zeros;
    nc.I8.zeros = &__zerosi8;
    nc.I8.where = &__where;
    nc.I.fill = &__fill;
    // nc.scalar = &std_scalar;
    return nc;
}

#endif
