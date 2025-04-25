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
void print_lines(XArray exarr) {
	// We assume array is only two dimensional
	int lines = exarr.shape.s.sh[0];
	int cols = exarr.shape.s.sh[1];
	int64_t pos = 0;
	int * arr = ((int*) exarr.array);
	for (int i=0; i<lines; i++) {
		for (int j=0; j<cols; j++) {
			pos = i * cols + j;
			printf("%i ", arr[pos]);
			
		}
		printf("\n");
	}
}
void fill_lines(XArray exarray, double val) {
	int lines = exarray.shape.s.sh[0];
	int cols = exarray.shape.s.sh[1];
	int64_t pos = 0;
	int * arr = ((int*) exarray.array);
	for (int i=0; i<lines; i++) {
		for (int j=0; j<cols; j++) {
			pos = i * cols + j;
			arr[pos] = (int)val;
		}
	}
}
int main(){
    printf("NumC testsuite!\n");
    NumC nc = numcinit(); // "import" numpy
    const int TEST_ARR_LEN = 2;
    XArray arr = nc.zeros(SHAPE(TEST_ARR_LEN, 1, 1, 1), INT);
    XArray randarr = nc.randint(SHAPE(TEST_ARR_LEN, 4, 1, 1), INT);
    // nc.fill(arr, 4);
    print_lines(randarr);
    fill_lines(arr, 4);
    print_lines(arr);
    nc.fill(arr, 6);

    // nc.shape(arr);

    return 0;
}

