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
} Sh;


// can be used to find a value for exmaple
typedef struct {
	size_t idx;
	double val;
} Tuple;

#define SHAPE(a, b, c, d) \
    ((Sh) {.sh[0]=a, .sh[1]=b, .sh[2]=c, .sh[3]=d})

typedef enum {
    INT,
    FLT,
    DBL
} Type;

typedef struct {
	int len;
	Sh s;
	Type type;
} XShape;

typedef struct {
	XShape shape;
	void * array;
} XArray;

typedef struct {
    XArray (*randint)(Sh s, Type type);
    XArray (*zeros)(Sh s, Type type);
    XArray (*arange)(double start, double stop, int len, Type type);
    double (*max)(void * array, int len, Type type); // should take any array
    double (*dot)(XArray a1, XArray a2);
    void (*fill)(XArray array, double val);
    void (*shape)(XArray array);
} NumC;


NumC numcinit();

static int * __intcast(void * array) {
	return (int*)array;
}

static double * __doublecast(void * array) {
	return (double*)array;
}

static float * __floatcast(void * array) {
	return (float*)array;
}

static void check_shape(Sh s) {
	// loop over shape	
	for (int i=0; i<4; i++){
		if (s.sh[i] < 0) {
			printf("ERROR: Negative shapes are not possible!\n");
			exit(1);
		}
	}
}


int64_t el_from_shape(Sh s) {
	int64_t elements;
	elements = s.sh[0] * s.sh[1] * s.sh[2] * s.sh[3];
	return elements;
}

XArray __zeros(Sh s, Type type){
	check_shape(s);
	XArray xarray = {{s.sh[0], s, type}, NULL};
	int64_t elements = el_from_shape(s);

	switch(type) {
		case INT:
			xarray.array = calloc(elements, sizeof(int));
			break;
		case FLT:
			xarray.array = calloc(elements, sizeof(float));
			break;
		case DBL:
			xarray.array = calloc(elements, sizeof(double));
			break;
	}
	return xarray;

}

XArray rint_(Sh s, Type type){
    XArray array = __zeros(s, type);
    int64_t elements = el_from_shape(s);
    srand(time(NULL));
    int * rarr = (int*)array.array;
    for (int i=0; i<elements; i++){
        rarr[i] = rand();
    }
    return array;
}

char * get_typestr(Type type) {
	char * name[8];
	switch (type) {
		case INT:
			name[0] = "int";
			break;
		case FLT:
			name[0] = "float";
			break;
		case DBL:
			name[0] = "double";
			break;
	}
	return name[0];
}

void shape(XArray array) {
	char * typename = get_typestr(array.shape.type);
	printf("SHAPE OF ARRAY\n");
	printf("Arraylen: %i\nArraytype: %s\n", array.shape.len, typename);
}

void __fill(XArray array, double val) {
	int64_t elements = el_from_shape(array.shape.s);
	printf("Filling in for %li elements\n", elements);
	int * arr = (int*)array.array;
	for (int i=0; i<elements; i++){
		arr[i] = (int)val;
	}
	
}
// TODO: arange does usually return int and gets either start, or start, stop, step
// This is rather like np.linspace and should therefore be renamed
XArray __arange(double start, double stop, int len, Type type) {
	XArray array = __zeros(SHAPE(len, 1, 1, 1), type);
	double * locarr = (double*)array.array;

	printf("DEBUG: Aranging array of len: %i and type %s\n",
			array.shape.len, get_typestr(type));
	int diff = stop - start;
	double stepwidth = (double)diff / array.shape.len;
	printf("DEBUG: Pointer array: %p \n", array.array);
	for (int i=0; i<array.shape.len; i++){
		printf("DEBUG: Pointer is at %p\n", array.array);
		locarr[i] = start + (stepwidth * i);
	}
	printf("DEBUG: Pointer before returning %p\n", array.array);
	return array;
}

double __sum(XArray array) {
	double sum = 0;

	if (array.shape.type == INT){
		int * locarr = (int*)array.array;
		for (int i=0; i<array.shape.len; i++){
			sum += locarr[i];
		}
	}	
	return sum;
}

double __max(void * array, int len, Type type) {
	double max = 0;
	int8_t * localarray = (int8_t *)array;
	for (int i=0; i<len; i++) {
		if (localarray[i] > max) {
			max = localarray[i];
		}
	}
	
	return max;
}
// std scalar product for arbitray types with void * pointers
double __std_scalar(XArray a1, XArray a2){

    	double sum = 0;
    // check the shapes
	int * locarr1 = (int*)a1.array;
	int * locarr2 = (int*)a2.array;
	for (int i=0; i<a1.shape.len; i++){
		sum += (double)locarr1[i] * (double)locarr2[i];
		printf("DEBUG: Sum is: %lf\n", sum);
	}    
    	return sum;
}

// how to clear this later?
NumC numcinit(){

    NumC nc;
    // nc.max = &__maxint;
    // nc.min = &__minint;
    nc.zeros = &__zeros;
    nc.randint = &rint_;
    nc.max = &__max;
    nc.dot = &__std_scalar;
    nc.fill = &__fill;
    nc.arange = &__arange;
    return nc;
}

#endif
