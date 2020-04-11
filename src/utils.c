// Sachin Shah
// March 10, 2020

#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void error(char *msg)
{
	fprintf(stderr, "%s\n", msg);
	exit(0);
}

void check_null(void *ptr, char *msg)
{
	if (ptr == NULL) error(msg);
}

void chomp(char *str)
{
	int i = strlen(str) - 1;

	while (isspace(str[i]) && i >= 0) str[i--] = '\0';
}