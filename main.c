#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "NumC.h"

/*
 * TODO
 * - sort out int vs. int8
 * - testing?
 * - multidimensional arrays implementation using shape
 * - reading into an array from a file pointer
 * - error handling
*/


int main(){
    printf("NumC testsuite!\n");
    NumC nc = numcinit(); // "import" numpy
    const int TEST_ARR_LEN = 7;
    XArray arr = nc.zeros(SHAPE(TEST_ARR_LEN, 1, 1, 1), INT);
    nc.shape(arr);

    return 0;
}

