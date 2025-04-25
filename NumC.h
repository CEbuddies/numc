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
    ((Shape) {.sh[0]=a, .sh[1]=b, .sh[2]=c, .sh[3]=d})

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

// standard array with a void pointer
typedef struct {
    int * array;
    int len;
    Type type;
} ArrayI;

typedef struct {
    int8_t * array;
    int len;
    Type type;
} ArrayI8;

typedef struct {
	XShape shape;
	void * array;
} XArray;

typedef struct {
    XArray (*randint)(int len);
    XArray (*zeros)(Sh s, Type type);
    double (*max)(void * array, int len, Type type); // should take any array
    double (*scalar)(void * a1, void * a2, int len, Type type);
    void (*fill)(void * array, int len, double val, Type type);
} NumC;


NumC numcinit();

static int * __intcast(void * array) {
	return (int*)array;
}



int64_t el_from_shape(Sh s) {
	int64_t elements;
	elements = s.sh[0] * s.sh[1] * s.sh[2] * s.sh[3];
	return elements;
}

XArray __zeros(Sh s, Type type){
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
    srand(time(NULL));
    int * rarr = (int*)array.array;
    for (int i=0; i<array.shape.len; i++){
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

ArrayI __whereAll(ArrayI8 array, int8_t val) {
	int count = 0;
	int loc_cap = 15;
	int * locs = calloc(loc_cap, sizeof(int));
	for (int i=0; i<array.len; i++) {
		if (array.array[i] == val) {
			count++;
			if (count > loc_cap) {
				loc_cap *= 2;
				locs = realloc(locs, loc_cap*sizeof(int));
			}
			locs[count-1] = i;
		}
		
	}
	ArrayI arr;
	arr.array = locs;
	arr.len = count;
	arr.type = INT;
	return arr;
}

void __filli8(ArrayI8 array, int8_t val){
	for (int i=0; i<array.len; i++) {
		array.array[i] = val;
	}
}

void __fill(void * array, int len, double val, Type type){

	switch(type) {
		case INT:
			{
				int ival = (int)val;
				for (int i=0; i<len; i++) {
					((int8_t *)array)[i] = ival;
				}
				break;
			}
		case FLT:
			{
				float fval = (float)val;
				for (int i=0; i<len; i++) {
					((float *)array)[i] = fval;
				}
				break;
			}
		case DBL:
			{
				double dval = (double)val;
				for (int i=0; i<len; i++) {
					((double *)array)[i] = dval;
				}
				break;
			}
	}
	
}

int64_t __counti8(ArrayI8 array, int8_t val) {
	int64_t count = 0;
	for (int i=0; i<array.len; i++) {
		if (array.array[i] == val) {
			count++;
		}
	}
	return count;
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
double __std_scalar(void * a1, void * a2, int len, Type type){

    double sum = 0;
    if (type == INT){
	for (int i=0; i<len; i++){
	    sum += ((int8_t *)a1)[i] * ((int8_t *)a2)[i];
	}
    } else if (type == DBL){
	for (int i=0; i<len; i++){
	    sum += ((double *)a1)[i] * ((double *)a2)[i];
	}
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
    nc.scalar = &__std_scalar;
    nc.I.zeros = &__zeros;
    nc.I8.zeros = &__zerosi8;
    nc.I8.where = &__where;
    nc.I8.whereAll = &__whereAll;
    nc.I8.fill = &__filli8;
    nc.I8.count = &__counti8;
    nc.I.fill = &__fill;
    // nc.scalar = &std_scalar;
    return nc;
}

#endif
