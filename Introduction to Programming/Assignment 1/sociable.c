#include <stdio.h>

#define MAXNUM 10000000
#define MAXCYC 28

// Find the sum of divisors of a number based on its prime factorization.
int sumOfDivisors(int x) {
	int i, j;
	int d, prod;

	// Some parts of this function could be written more compressed by using loops, but since the loops would be executed too few times,
	// the execution time would rise for no reason, and so I have hardcoded them.
	// NOTE(stefanos): With compiler optimizations, this would be done automatically as loop unrolling.
	
	// Check if 2 or 3 divide the number
	prod = 1;
	d = 1;
	while(x % 2 == 0) {
		d = (d * 2) + 1;
		x /= 2;
	}
	prod *= d;

	d = 1;
	while(x % 3 == 0) {
		d = (d * 3) + 1;
		x /= 3;
	}
	prod *= d;
	
	// All primes except 2 and 3 are in the form 6k +/- 1.
	for(i = 5; i*i <= x; i += 6) {
		j = i;
		d = 1;
		while(x % j==0) {
			d = (d * j) + 1;
			x = x / j;
		}
		prod = prod * d;

		j += 2;
		d = 1;
		while(x % j == 0) {
			d = (d * j) + 1;
			x = x / j;
		}
		prod = prod * d;
	}

	if(x > 1) {    // x is the only prime bigger than square root and it is in exponent 1.
		prod *= x + 1;
	}

	return prod;
}

int main(void) {
	
	int i, j;
	int general, count, per, am;
	
	general = am = per = 0;
	for(i = 2; i <= MAXNUM; ++i) {
		count = 0;
		j = i;
		do {
			j = sumOfDivisors(j) - j;
			++count;
		} while((j > i) && (j <= MAXNUM) && (count <= MAXCYC));     
		// always start from the smallest number in the cycle. If i is not the smallest number in the cycle,
		// then we have already printed the cycle which contains i (if there exists a cycle with i in it).

		if(j == i) {    // we have a finished cycle
			if(count == 2)
				++am;
			else if(count > 2) 
				++general;
			else 
				++per;

			printf("Cycle of length %d: %d ", count, i);
			while(count > 1) {    // take the same procedure as before to print the intermediate numbers of the cycle.
				j = sumOfDivisors(j) - j;
				printf("%d ", j);
				--count;
			}
			printf("%d\n", i);
		}
	}

	printf("\n\nFound %d cycles including\n", general + am + per);
	printf("%d amicable and %d perfect cycles\n", am, per);
	
	
	return 0;
}
