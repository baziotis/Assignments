#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "misc.h"
#include "dtw.h"

int main(int argc, char **argv) {
	
	long int i, j;
	int c;
	double *B, **train_set;
	double *cache, *cache_prev;
	double min_distance, dist;
	int num_train, num_test, near_series, curr_class, fails;
	int *train_classes;
	long int len_train, len_test;
	clock_t begin, end;
	double time_spent;

	begin = clock();
	
	c = -1;
	if(argc > 1) {   // we have constraint
		c = atoi(argv[1]);
		if(c < 0) {
			fprintf(stderr, "Negative argument as constraint\n");
			return CONST_ERROR;
		}
	}
	
	if(scanf("%d", &num_train) != 1) {  // read the number of training series
		fprintf(stderr, "Error while reading the number of training series\n");
		return READ_ERROR;
	}
	if(scanf("%ld", &len_train) != 1) {  // read the length of the training series
		fprintf(stderr, "Error while reading the length of every training series\n");
		return READ_ERROR;
	}

	
	// Allocate memory for the training set and training classes and read its contents
	train_set = malloc(num_train * sizeof(double*));
	if(train_set == NULL) {
		fprintf(stderr, "ERROR: Memory error\n");
		return MEM_ERROR;
	}
	train_classes = malloc(num_train * sizeof(int));
	if(train_classes == NULL) {
		fprintf(stderr, "ERROR: Memory error\n");
		return MEM_ERROR;
	}
	for(i = 0; i < num_train; i++) {
		train_set[i] = malloc(len_train * sizeof(double));
		if(train_set[i] == NULL) {
			fprintf(stderr, "ERROR: Memory error\n");
			return MEM_ERROR;
		}
		if(scanf("%d", train_classes + i) != 1) {  // read the training class
			fprintf(stderr, "Error while reading the class of a traning series\n");
			return READ_ERROR;
		}
		for(j = 0; j < len_train; j++) {   // read the training series
			if(scanf("%lf", train_set[i] + j) != 1) {
				fprintf(stderr, "Error while reading the contents of a traning series\n");
				return READ_ERROR;
			}
		}
	}

	if(scanf("%d", &num_test) != 1) {  // read the number of test series
		fprintf(stderr, "Error while reading the number of test series\n");
		return READ_ERROR;
	}
	if(scanf("%ld", &len_test) != 1) {  // read the length of test series
		fprintf(stderr, "Error while reading the length of every test series\n");
		return READ_ERROR;
	}

	if(len_test != len_train) {   // difference in the length of train and test series - Error
		fprintf(stderr, "Different lengths on the test set and the training set\n");
		return 1;
	}

	if(c < 0)   // no constraint was given
		c = len_train;
	
	
	// 'B' is an array where we will store the contents of every new test
	// series and test it against the whole training set
	B = malloc(len_test * sizeof(double));
	if(B == NULL) {
		fprintf(stderr, "ERROR: Memory error\n");
		return MEM_ERROR;
	}
	cache = malloc((2 * c + 1) * sizeof(double));
	if(cache == NULL) {
		fprintf(stderr, "ERROR: Memory error\n");
		return MEM_ERROR;
	}
	cache_prev = malloc((2 * c + 1) * sizeof(double));
	if(cache_prev == NULL) {
		fprintf(stderr, "ERROR: Memory error\n");
		return MEM_ERROR;
	}
	fails = 0;   // number of failed class reports
	for(i = 0; i < num_test; i++) {
		if(scanf("%d", &curr_class) != 1) {   // read the class of the current test series
			fprintf(stderr, "Error while reading the class of a test series\n");
			return READ_ERROR;
		}
		
		// read the contents of the series
		for(j = 0; j < len_test; j++) {
			if(scanf("%lf", B + j) != 1) {
				fprintf(stderr, "Error while reading the contents of a test series\n");
				return READ_ERROR;
			}
		}

		// Find the training series which has the min distance with the current test series
		min_distance = INF;
		for(j = 0; j < num_train; ++j) {
			dist = dtw(train_set[j], B, len_test, c, cache, cache_prev);
			if(dist < min_distance) {
				min_distance = dist;
				near_series = j + 1;
			}
		}

		// Difference in class of the current test series with the class of the training
		// series which gives the minium distance - We have a failure
		if(train_classes[near_series - 1] != curr_class)
			fails++;
		printf("Series %ld (class %d) is nearest (distance %lf) to series %d (class %d)\n", i + 1, curr_class, min_distance, near_series, train_classes[near_series - 1]);
	}

	
	// free allocated memory
	// We assume that because of the previous tests
	// the following pointers will never be NULL
	if(cache != NULL)   
		free(cache);
	if(cache_prev != NULL) 
		free(cache_prev);
	if(B != NULL)
		free(B);
	for(i = 0; i < num_train; ++i) {
		if(train_set[i] != NULL)
			free(train_set[i]);
	}
	if(train_set != NULL)
		free(train_set);
	
	// Print the error rate
	fprintf(stderr, "Error rate: %.3lf\n", ((double)fails) / num_test);
	
	// Print the CPU time
	end = clock();
	time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	fprintf(stderr, "CPU Time: %.2lf\n", time_spent);

	return 0;
}
