#ifndef GUARD_MISC   // if supported, use #pragma once
#define GUARD_MISC

#define INF (1.0/0.0) // NOTE: be careful with what you use as infinity

// Errors
#define CONST_ERROR 1
#define READ_ERROR -1
#define MEM_ERROR -2

// macro functions for simple computations
#define max(a, b) ((a) > (b) ? (a) : (b))
#define d(a, b) ((a - b) * (a - b))
#define min(a, b) ((a) < (b) ? (a) : (b))

#endif
