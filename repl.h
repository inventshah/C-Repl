#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define typef(x) _Generic((x), \
	unsigned char: "c"           , char: "c",            \
	signed char: "c"             , short int: "hi",      \
	unsigned short int: "hu"     , int: "d",             \
	unsigned int: "i"            , long int: "l",        \
	unsigned long int: "lu"      , long long int: "lli", \
	unsigned long long int: "llu", float: "f",           \
	double: "f"                  , long double: "Lf",    \
	char *: "s"                  , void *: "p",          \
	default: "p")
