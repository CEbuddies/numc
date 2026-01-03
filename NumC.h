#include <stddef.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

// TODO: Indexing, e.g. make a __index function taking lines and cols

#ifndef NUMC_H
#define NUMC_H

typedef union {
	struct {
		int s0;
		int s1;
	} shapef;
	int sh[2];
} Sh;


// can be used to find a value for exmaple
typedef struct {
	size_t idx;
	double val;
} Tuple;

typedef struct {
	size_t a;
	size_t b;
} TwoTuple;

#define SHAPE(a, b) \
	((Sh) {.sh[0]=a, .sh[1]=b})

typedef enum {
	INT,
	FLT,
	DBL
} Type;

typedef struct {
	size_t len;
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
	XArray (*linspace)(double start, double stop, int len, Type type);
	XArray (*arange)(int start, int stop, int step);
	XArray (*rand)(Sh s);
	XArray (*cumsum)(XArray array);
	double (*max)(XArray array); // should take any array
	double (*min)(XArray array);
	double (*dot)(XArray a1, XArray a2);
	double (*sum)(XArray array);
	void (*fill)(XArray array, double val);
	void (*shape)(XArray array);
	int (*free)(XArray array);
	void (*print)(XArray array);
	void (*set)(XArray array, Sh s, double val);
	double (*get)(XArray array, Sh s);
} NumC;


NumC numcinit();

static int * __intcast(XArray array) {
	return (int*)array.array;
}

static double * __doublecast(XArray array) {
	return (double*)array.array;
}

static float * __floatcast(XArray array) {
	return (float*)array.array;
}

static void check_shape(Sh s) {
	// loop over shape
	for (int i=0; i<2; i++){
		if (s.sh[i] < 0) {
			printf("ERROR: Negative shapes are not possible!\n");
			exit(1);
		}
	}
}

int __free(XArray array) {
	free(array.array);
	return 0;
}


int64_t el_from_shape(Sh s) {
	int64_t elements;
	elements = s.sh[0] * s.sh[1];
	return elements;
}

void __set(XArray array, Sh s, double val) {

	size_t idx = el_from_shape(s);
	if (idx >= array.shape.len) {
		printf("ERROR: Index out of bounds\n");
		exit(1);
	}
	switch(array.shape.type) {
		case INT:
			__intcast(array)[idx] = (int)val;
			break;
		case FLT:
			__floatcast(array)[idx] = (float)val;
			break;
		case DBL:
			__doublecast(array)[idx] = val;
			break;
	}
}

double __get(XArray array, Sh s) {
	size_t numel = el_from_shape(s);
	if (numel > array.shape.len) {
		printf("ERROR: Index out of bounds\n");
		exit(1);
	}

	switch(array.shape.type) {
		case INT:
			return __intcast(array)[numel];
			break;
		case FLT:
			return __floatcast(array)[numel];
			break;
		case DBL:
			return __doublecast(array)[numel];
			break;
	}
}

XArray __zeros(Sh s, Type type){
	check_shape(s);
	XArray xarray = {{0, {s.sh[0], s.sh[1]}, type}, NULL};
	int64_t elements = el_from_shape(s);
	xarray.shape.len = elements;

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

XArray rand_(Sh s) {
	XArray array = __zeros(s, DBL);
	int64_t elements = el_from_shape(s);
	srand(time(NULL));
	double * rarr = (double*)array.array;
	for (int i=0; i<elements; i++){
		rarr[i] = rand() * 1.0 / RAND_MAX;
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


	switch (array.shape.type) {
		case INT:
			{
			int * arr = __intcast(array);
			for (int i=0; i<elements; i++){
				arr[i] = (int)val;
			}
			break;
			}
		case FLT:
			{
			float * arr = __floatcast(array);
			for (int i=0; i<elements; i++){
				arr[i] = (float)val;
			}
			break;
			}
		case DBL:
			{
			double * arr = __doublecast(array);
			for (int i=0; i<elements; i++){
				arr[i] = val;
			}
			break;
			}
	}

}
XArray __linspace(double start, double stop, int len, Type type) {
	XArray array = __zeros(SHAPE(len, 1), type);
	double * locarr = (double*)array.array;

	int diff = stop - start;
	double stepwidth = (double)diff / array.shape.len;
	for (int i=0; i<array.shape.len; i++){
		locarr[i] = start + (stepwidth * i);
	}
	return array;
}

/**
 * @brief Returns a 1D of type INT aranged from start to stop with spacing step
 *
 * @param[int] start First number of aranged items
 * @param[int] stop Last number to be included
 * @param[int] step Step width between two items
 * @return XAarray
 */
XArray __arange(int start, int stop, int step) {
	XArray array = __zeros(SHAPE(stop-start, 1), INT);
	int * locarr = (int*)array.array;
	for (int i=0; i<array.shape.len; i++){
		locarr[i] = start + (step * i);
	}
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

// MAX
double __maxint(int * array, size_t len) {
	double max = 0;
	for (int i=0; i<len; i++){
		if (array[i] > max) {
			max = array[i];
		}
	}
	return max;
}

double __maxdbl(double * array, size_t len) {
	double max = 0;
	for (int i=0; i<len; i++){
		if (array[i] > max) {
			max = array[i];
		}
	}
	return max;
}

double __max(XArray array) {
	double max = 0;

	switch (array.shape.type) {
		case INT:
			{
			int * locarray = (int*)array.array;
			max = __maxint(locarray, array.shape.len);
			break;
			}
		case FLT:
			{
			max = __maxdbl(__doublecast(array), array.shape.len);
			}
			break;
		case DBL:
			{
			max = __maxdbl(__doublecast(array), array.shape.len);
			break;
			}
	}

	return max;
}

double __minint(int * array, size_t len) {
	double min = array[0];
	for (int i=0; i<len; i++){
		if (array[i] < min) {
			min = array[i];
		}
	}
	return min;
}

double __minflt(float * array, size_t len) {
	double min = array[0];
	for (int i=0; i<len; i++){
		if (array[i] < min) {
			min = array[i];
		}
	}
	return min;
}

double __mindbl(double * array, size_t len) {
	double min = array[0];
	for (int i=0; i<len; i++){
		if (array[i] < min) {
			min = array[i];
		}
	}
	return min;
}

//TODO: implement the respective mins
double __min(XArray array) {
	double min = 0;

	switch (array.shape.type) {
		case INT:
			{
			min = __minint(__intcast(array), array.shape.len);
			break;
			}
		case FLT:
			{
			min = __minflt(__floatcast(array), array.shape.len);
			}
			break;
		case DBL:
			{
			min = __mindbl(__doublecast(array), array.shape.len);
			break;
			}
	}
	return min;
}
// std scalar product for arbitray types with void * pointers
double __std_scalar(XArray a1, XArray a2){

	double sum = 0;
	// check the shapes
	int * locarr1 = (int*)a1.array;
	int * locarr2 = (int*)a2.array;
	for (int i=0; i<a1.shape.len; i++){
		sum += (double)locarr1[i] * (double)locarr2[i];
	}
	return sum;
}

void __csumInt(int * csarray, int * array, size_t len) {
	double csum_counter = 0;
	for (int i=0; i<len; i++){
		csum_counter += array[i];
		csarray[i] = csum_counter;
	}
}

void __csumDouble(double * csarray, double * array, size_t len) {
	double csum_counter = 0;
	for (int i=0; i<len; i++){
		csum_counter += array[i];
		csarray[i] = csum_counter;
	}
}

void __csumFloat(float * csarray, float * array, size_t len) {
	double csum_counter = 0;
	for (int i=0; i<len; i++){
		csum_counter += array[i];
		csarray[i] = csum_counter;
	}
}

XArray __cumsum(XArray array) {
	XArray csumarray = __zeros(array.shape.s, DBL);
	switch (array.shape.type) {
		case INT:
			{
			int * locarray = __intcast(array);
			__csumInt(__intcast(csumarray), locarray, array.shape.len);
			break;
			}
		case FLT:
			{
			float * locarray = __floatcast(array);
			__csumFloat(__floatcast(csumarray), locarray, array.shape.len);
			break;
			}
		case DBL:
			{
			double * locarray = __doublecast(array);
			__csumDouble(__doublecast(csumarray), locarray, array.shape.len);
			break;
			}
	}
	return csumarray;
}
// PRINTING
void __print_int(XArray array) {
	int * locarr = (int*)array.array;
	size_t nlines = array.shape.s.sh[0];
	size_t ncols = array.shape.s.sh[1];
	printf("Printing array of type INT{");
	printf("Shape(%d, %d)\n", array.shape.s.sh[0], array.shape.s.sh[1]);
	for (int i=0; i<nlines; i++){
		printf("  ");
		for (int j=0; j<ncols; j++){
			printf("%d ", locarr[i*array.shape.s.sh[0] + j]);
		}
		printf("\n");
	}
	printf("}\n");
}

void __print_float(XArray array) {
	float * locarr = __floatcast(array);
	size_t nlines = array.shape.s.sh[0];
	size_t ncols = array.shape.s.sh[1];
	printf("Printing array of type FLOAT{");
	printf("Shape(%d, %d)\n", array.shape.s.sh[0], array.shape.s.sh[1]);
	for (int line=0; line<nlines; line++) {
		printf("  ");
		for (int col=0; col<ncols; col++) {
			printf("%f ", locarr[line*ncols + col]);
		}
		printf("\n");
	}
	printf("}\n");
}


void __print_double(XArray array) {
	double * locarr = __doublecast(array);
	size_t nlines = array.shape.s.sh[0];
	size_t ncols = array.shape.s.sh[1];
	printf("Printing array of type DOUBLE{");
	printf("Shape(%d, %d)\n", array.shape.s.sh[0], array.shape.s.sh[1]);
	for (int line=0; line<nlines; line++) {
		printf("  ");
		for (int col=0; col<ncols; col++){
			printf("%f ", locarr[line * ncols + col]);
		}
		printf("\n");
	}
	printf("}\n");
}

void __print(XArray array) {
	switch (array.shape.type) {
		case INT:
			{
			__print_int(array);
			break;
			}
		case FLT:
			{
			__print_float(array);
			break;
			}
		case DBL:
			{
			__print_double(array);
			break;
			}
	}
}
// NumC Def

/**
 * @brief Init function for NumC that sets it up (import numpy as np)
 *
 * @return NumC nc struct (object) containing all functions
 */
NumC numcinit(){

	NumC nc;
	nc.zeros = &__zeros;
	nc.randint = &rint_;
	nc.rand = &rand_;
	nc.max = &__max;
	nc.min = &__min;
	nc.dot = &__std_scalar;
	nc.sum = &__sum;
	nc.fill = &__fill;
	nc.linspace = &__linspace;
	nc.cumsum = &__cumsum;
	nc.arange = &__arange;
	nc.set = &__set;
	nc.get = &__get;
	nc.free = &__free;
	nc.print = &__print;
	return nc;
}

// Plotting

typedef struct {
	bool (*scatter)(XArray x, XArray y);
	bool (*plot)(XArray x, XArray y);
	
}NcPlot;

bool scatter(XArray x, XArray y) {
    if (x.shape.len != y.shape.len) {
        fprintf(stderr, "Error: Arrays must have same length\n");
        return false;
    }
	double * array1 = __doublecast(x);
	double * array2 = __doublecast(y);
	size_t len = x.shape.len; // the length for all both array
    
    FILE *gp = popen("gnuplot -persist", "w");
    if (!gp) {
        fprintf(stderr, "Error: Could not open gnuplot.  Is it installed?\n");
        return false;
    }
    
    // Configure plot
    fprintf(gp, "set grid\n");
    fprintf(gp, "set style data lines\n");
    fprintf(gp, "plot '-' with points title 'data'\n");
    
    // Send data
    for (size_t i = 0; i < len; i++) {
        fprintf(gp, "%f %f\n", array1[i], array2[i]);
    }
    fprintf(gp, "e\n");
    
    fflush(gp);
    pclose(gp);
    return true;
}

bool plot(XArray x, XArray y) {
    if (x.shape.len != y.shape.len) {
        fprintf(stderr, "Error: Arrays must have same length\n");
        return false;
    }
	double * array1 = __doublecast(x);
	double * array2 = __doublecast(y);
	size_t len = x.shape.len; // the length for all both array
    
    FILE *gp = popen("gnuplot -persist", "w");
    if (!gp) {
        fprintf(stderr, "Error: Could not open gnuplot.  Is it installed?\n");
        return false;
    }
    
    // Configure plot
    fprintf(gp, "set grid\n");
    fprintf(gp, "set style data lines\n");
    fprintf(gp, "plot '-' with lines title 'data'\n");
    
    // Send data
    for (size_t i = 0; i < len; i++) {
        fprintf(gp, "%f %f\n", array1[i], array2[i]);
    }
    fprintf(gp, "e\n");
    
    fflush(gp);
    pclose(gp);
    return true;
}

NcPlot ncplotinit(){
	NcPlot ncplot;
	ncplot.scatter = &scatter;
	ncplot.plot = &plot;
	return ncplot;
}
#endif
