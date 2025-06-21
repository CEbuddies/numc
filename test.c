#include "NumC.h"
#include <stdio.h>

int main() {
	// Initialize NumC
	NumC nc = numcinit();

	// Test 1: Create and test zeros array
	printf("\n=== Test 1: Zeros array ===\n");
	XArray zeros_arr = nc.zeros(SHAPE(2, 3), INT);
	printf("Shape: %d %d %d %d\n", zeros_arr.shape.s.sh[0], 
	zeros_arr.shape.s.sh[1], 
	zeros_arr.shape.s.sh[2], 
	zeros_arr.shape.s.sh[3]);

	int *zero_data = __intcast(zeros_arr);
	for(int i = 0; i < 6; i++) {
		printf("%d ", zero_data[i]);
	}
	printf("\n");
	nc.free(zeros_arr);

	// Test 2: Test arange
	printf("\n=== Test 2: Arange ===\n");
	XArray arange_arr = nc.arange(0, 10, 2);
	int *arange_data = __intcast(arange_arr);
	printf("Arange values: ");
	for(int i = 0; i < 5; i++) {
		printf("%d ", arange_data[i]);
	}
	printf("\n");
	nc.free(arange_arr);

	// Test 3: Test linspace
	printf("\n=== Test 3: Linspace ===\n");
	XArray linspace_arr = nc.linspace(0.0, 1.0, 5, DBL);
	double *linspace_data = __doublecast(linspace_arr);
	printf("Linspace values: ");
	for(int i = 0; i < 5; i++) {
		printf("%.2f ", linspace_data[i]);
	}
	printf("\n");
	nc.free(linspace_arr);

	// Test 4: Test sum and max
	printf("\n=== Test 4: Sum and Max ===\n");
	XArray test_arr = nc.zeros(SHAPE(3, 1), INT);
	nc.fill(test_arr, 5);

	printf("Sum: %.1f\n", nc.sum(test_arr));
	printf("Max: %.1f\n", nc.max(test_arr));
	nc.free(test_arr);

	// Test 5: Test dot product
	printf("\n=== Test 5: Dot Product ===\n");
	XArray a1 = nc.zeros(SHAPE(3, 1), INT);
	XArray a2 = nc.zeros(SHAPE(3, 1), INT);

	nc.fill(a1, 1);
	nc.fill(a2, 2);

	double dotrest = nc.dot(a1, a2);
	printf("Dot product: %lf\n", dotrest);
	nc.free(a1);
	nc.free(a2);

	printf("\n=== Test 6: Random doubles ===\n");
	XArray rand_dbl = nc.rand(SHAPE(3, 1));
	double *rand_dbl_data = __doublecast(rand_dbl);
	printf("Random doubles: ");
	for(int i = 0; i < 3; i++) {
		printf("%.2f ", rand_dbl_data[i]);
	}
	printf("\n");
	nc.free(rand_dbl);

	printf("\n=== Test 7: Cumsum ===\n");
	XArray lsp = nc.linspace(0.0, 1.0, 5, DBL);
	XArray csum = nc.cumsum(lsp);
	nc.print(lsp);
	nc.print(csum);

	nc.free(lsp);
	nc.free(csum);

	printf("\n=== Test 8: Error handling ===\n");
	XArray fail = nc.zeros(SHAPE(10, -1), DBL);



	return 0;
}
