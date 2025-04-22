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
    const int TEST_ARR_LEN = 5;
    ArrayI Iempty = nc.I.zeros(TEST_ARR_LEN);
    ArrayF Fempty = nc.Fl.zeros(TEST_ARR_LEN);
    nc.I.fill(Iempty, 3);

    printf("Checking, value number 3 from empty array: %i\n", Iempty.array[3]);

    return 0;
}

