#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

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

#define SHAPE(a, b, c, d) \
    ((Shape) {.sh[0]=a, .sh[1]=b, .sh[2]=c, .sh[3]=d})

typedef enum {
    INT,
    FLT,
    DBL
} Type;

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
    float * array;
    int len;
    Type type;
} ArrayF;

typedef struct {
    void * array;
    int len;
    Type type;
} ArrayG;

typedef struct {
	ArrayI (*zeros)(int len);
	void (*fill)(ArrayI array, int8_t val);
} Int;

typedef struct {
	ArrayF (*zeros)(int len);
	ArrayF (*fill)(ArrayF array, float val);
} Float;

typedef struct {
    Array (*randint)(int len);
    ArrayI (*zeros)(int len); 
    ArrayF (*zerosf)(int len);
    int (*max)(Array array); // should take any array
    int (*min)(Array array);
    double (*scalar)(Array a1, Array a2);
    Int I;
    Float Fl;
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
// TODO: might need typecast or something
int __maxint(int * array, int len){
    // how to go about overflows?
    int max = array[0];
    for (int i=0; i<len; i++){
        if (array[i] > max){
            max = array[i];
        }
    }
    return max;
}

int __minint(int * array, int len){
    int min = array[0];
    for (int i=0; i<len; i++){
        if (array[i] < min){
            min = array[i];
        }
    }
    return min;
}

ArrayI __zeros(int len){
	ArrayI array;
	array.len = len;
	array.type = INT;
	array.array = calloc(len, sizeof(int8_t));

	return array;
}

ArrayI8 __zerosG(int len){
	ArrayI8 array;
	array.len = len;
	array.type = INT;
	array.array = calloc(len, sizeof(int8_t));
}

ArrayF __zerosf(int len){
	ArrayF array;
	array.len = len;
	array.type = FLT;
	array.array = calloc(len, sizeof(float));

	return array;
}
void __fill(ArrayI array, int8_t val){
	for (int i=0; i<array.len; i++) {
		array.array[i] = val;
	}
}

// Fill function for floats
void __fillf(ArrayF array, float val){
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

// how to clear this later?
NumC numcinit(){

    NumC nc;
    // nc.max = &__maxint;
    // nc.min = &__minint;
    nc.zeros = &__zeros;
    nc.zerosG = &__zerosG;
    nc.zerosf = &__zerosf;
    nc.randint = &rint_;
    nc.I.zeros = &__zeros;
    nc.I.fill = &__fill;
    nc.Fl.zeros = &__zerosf;
    nc.Fl.fill = &__fillf;
    // nc.scalar = &std_scalar;
    return nc;
}

#endif
