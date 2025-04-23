#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "NumC.h"

void parse_matstring(char * matstr){
    for (int i=0; i<strlen(matstr); i++){
        printf("Char number %d: %c\n", i, matstr[i]);
    }
}

typedef struct {
    int a;
    int b;
} SomeStruct;
    
Array return_array(void * array, int len){
    Array arr = {
        .array = array,
        .len = len
    };
    return arr;
}

int main(){
    printf("NumC testsuite!\n");
    NumC nc = numcinit(); // "import" numpy
    const int TEST_ARR_LEN = 7;
    ArrayI8 emptyi8 = nc.I8.zeros(TEST_ARR_LEN);
    nc.I8.fill(emptyi8, 4);

    size_t count = nc.I8.count(emptyi8, 4);
    printf("Count of 4 in i8 array: %ld\n", count);
    double scalar = nc.scalar(emptyi8.array, emptyi8.array, emptyi8.len, emptyi8.type);
    printf("Standard scalar product is: %lf\n", scalar);
    emptyi8.array[3] = 6;
    emptyi8.array[1] = 6;
    ArrayI ari = nc.I.zeros(TEST_ARR_LEN);
    ArrayI locs = nc.I8.whereAll(emptyi8, 6);
    double max = nc.max(emptyi8.array, emptyi8.len, emptyi8.type);
    printf("Len of locs: %i\n", locs.len);
    printf("Max found to be: %lf\n", max);

    return 0;
}

