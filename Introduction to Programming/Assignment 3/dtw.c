#include <stdlib.h>
#include "misc.h"

double dtw(double *A, double *B, long int len, int c, double* cache, double* cache_prev) {
	long int i, j, k, start, end;
	double res;
	double *cache_temp;
	double x, y, z;

	// caches should be 2c + 1 in size.
	
	// Using only two caches of size len(i.e N). That means we have space complexity = O(2N) = O(N)
	// Every time, cache is the current line of the grid and cache_prev the previous line.
	// The two caches were initialized by main
	// If we have a constant c, then in every line of the grid, we have c items on right of the main diagonal, c items on the left
	// plus 1 item, the item ON the main diagonal, so we have O(c) complexity.
	for(i = 0; i < 2*c + 1; i++)
		cache[i] = cache_prev[i] = INF;
	
	cache[c] = d(A[0], B[0]);
	for(i = 0; i < len; ++i) {
		start = max(0, i - c);
		end = min(i + c, len - 1);
		k = max(0, c - i);
		if(i == 0) {
			start = 1;
			k++;
		}
		for(j = start; j <= end; ++j, ++k) {
			if(j == 0 || k == 0)     
				x = INF;
            	else                      
				x = cache[k - 1];

            	if(i == 0 || (k > 2*c - 1))   
				y = INF;
            	else                      
				y = cache_prev[k + 1];

            	if(j == 0)     
				z = INF;
            	else                      
				z = cache_prev[k];

			cache[k] = d(A[i], B[j]) + min( min(x, y), z );
		}

		// swap the caches
		cache_temp = cache;
		cache = cache_prev;
		cache_prev = cache_temp;
	}
	
	return cache_prev[k - 1];
}
