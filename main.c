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

int main(){
	printf("NumC testsuite!\n");
	NumC nc = numcinit(); // "import" numpy
	const int TEST_ARR_LEN = 2;
	// nc.fill(arr, 4);
	XArray linspaced = nc.linspace(0, 10, 10, DBL);
	XArray aranged = nc.arange(0, 6, 1);
	double * testarr = (double*)aranged.array;
	printf("Testarr: %lf\n", testarr[3]);


	XArray dot1 = nc.zeros(SHAPE(10, 10), INT);
	XArray dot2 = nc.zeros(SHAPE(10, 10), INT);
	nc.fill(dot1, 1);
	nc.fill(dot2, 2);
	double dotres = nc.dot(dot1, dot2);
	printf("Dot result: %lf\n", dotres);
	nc.free(linspaced);
	printf("Sum aranged: %lf\n", nc.sum(aranged));
	XArray fail = nc.zeros(SHAPE(10, -1), DBL);



	// nc.shape(arr);

	return 0;
}

