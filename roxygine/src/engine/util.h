#ifndef RG_UTIL_H
#define RG_UTIL_H

#include <stdio.h>

#define ERROR_EXIT(e, ...) { fprintf(stderr, __VA_ARGS__); exit(e); }
#define ERROR_RETURN(r, ...) { fprintf(stderr, __VA_ARGS__); return r; }

#endif