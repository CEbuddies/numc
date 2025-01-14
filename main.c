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

    Array r_a = nc.randint(TEST_ARR_LEN);
    Array r_a2 = nc.randint(TEST_ARR_LEN);
    ArrayD a_d = nc.ranD(23);
    r_a2.array[0] = -120;
    // This can be done nicer
    add_ref(&nc, &r_a); 
    //for (int i=0; i<TEST_ARR_LEN; i++){
    //    printf("El %d: %d\n", i, r_a.array[i]);
    //}

    printf("Max is: %d\n", nc.max(r_a));
    printf("Min is: %d\n", nc.min(r_a));
    printf("Scalar product: %lf\n", nc.scalar(r_a2, r_a));

    printf("Parsing ... \n");
    int dim0, dim1, dim2;
    sscanf("(23, 4,6)", "(%d, %d, %d)", &dim0, &dim1, &dim2);

    printf("Loop through string ...\n");
    char somestr[] = "123";
    parse_matstring(somestr);
    printf("Dimensions: %d, %d, %d\n", dim0, dim1, dim2);
    SomeStruct ss = {
        .a = 4,
        .b = 5
    };

    // void pointer array returning
    clean_up(&nc); 
    return 0;
}

