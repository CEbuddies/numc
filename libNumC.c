#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "NumC.h"

// TODO: Variable number of args for multi dim array, with a string?
// TODO: Use void pointer and make functions work on the fly
// TODO: Matrices have orientation, 4x3 not same as 3x4
// TODO: Make multi-dim arrays even work!
// TODO: Multi datatype versions
// TODO: Check if we can use a macro for shape and type
// TODO: Build libs and link
// TODO: Make the ref list a linked list? Not so neccessary. Will be dealloc anyway
//
// TIPP: zeros function, that returns generic but Array type is specified
// useless!

static Array make_Array(int * array, int len){
    Array newArray =
    {
        .array = array, 
        .len = len // TODO: needs to work with shape somehow
    };

    return newArray;
}

// takes in a string of shape (n, m, k)
void shape_str(char * shapestr){
}

size_t calc_bytes(Shape sh, int dims, Type t){
    int bytes;
    switch(t){
        case INT:
            bytes = 4;
            break;
        case FLT:
            bytes = 4;
            break;
        case DBL:
            bytes = 8;
            break;
    }
    
    

}

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

Array rint_(int len){
    int * array = calloc(len, sizeof(int));
    srand(time(NULL));
    int r;
    for (int i=0; i<len; i++){
        array[i] = rand();
    }
    Array r_array = make_Array(array, len);
    // would like to make this a function but causes pointer troubles
    
    return r_array;
}

ArrayD _zeros(Shape sh){
    int dimension = 1;
    if (sh.shape1 > 0){
        dimension = 2;
    }
    if (sh.shape2 > 0){
        dimension = 3;
    }
    if (sh.shape3 > 0){
        dimension = 4;
    }

    return r_a;
}

ArrayD ranD(int len){
    double * array = calloc(len, sizeof(double));
    srand(time(NULL));
    for (int i=0; i<len; i++){
        array[i] = (double)rand()/RAND_MAX;
    }
    ArrayD r_a = 
    {
        .array = array,
        .len = len
    };
    return r_a;
}

ArrayF ranF(int len){
    float * array = calloc(len, sizeof(float));
    srand(time(NULL));
    for (int i=0; i<len; i++){
        array[i] = (float)rand()/RAND_MAX;
    }
    ArrayF r_a = 
    {
        .array = array,
        .len = len
    };
    return r_a;
}

int maxint(Array array){
    // how to go about overflows?
    int max = array.array[0];
    for (int i=0; i<array.len; i++){
        if (array.array[i] > max){
            max = array.array[i];
        }
    }
    return max;
}

int minint(Array array){
    int min = array.array[0];
    for (int i=0; i<array.len; i++){
        if (array.array[i] < min){
            min = array.array[i];
        }
    }
    return min;
}

int * add_array(Array * array){
    return array->array;
}

// how to clear this later?
NumC numcinit(){

    NumC nc;
    nc.max = &maxint;
    nc.min = &minint;
    nc.randint = &rint_;
    nc.scalar = &std_scalar;
    nc.ranD = &ranD;
    nc.ptr_list = calloc(10, sizeof(int*)); // FIXME: This must be variable
    nc.ptr_counter= 0;
    nc.len_ptr_list = 10;
    return nc;
}

static void update_pointer_count(NumC * nc){
    if (nc->ptr_counter + 1 >= nc->len_ptr_list){
        nc->ptr_list = realloc(nc->ptr_list, 2 * nc->len_ptr_list * sizeof(int*));
        nc->len_ptr_list = 2 * nc->len_ptr_list;
    }
    nc->ptr_counter += 1; // what to do if realloc fails?
}

// Add array to ref counter - not too bad if forgotten
void add_ref(NumC * nc, Array * array){
    update_pointer_count(nc);
    nc->ptr_list[nc->ptr_counter] = array->array;
}

int clean_up(NumC * nc){
    for (int i=0; i< nc->ptr_counter; i++){
        free(nc->ptr_list[i]);
    }
    return 1; // when do we alter this?
}
