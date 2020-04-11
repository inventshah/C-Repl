#include "dylib.h"
#include "read.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	char *str;
	uint32_t num = 0;
	init_loader();

	while (1)
	{
		str = read();
		if (strcmp(str, "quit;") == 0) break;

		if (eval(str, num++) == NULL) num--;
		free(str);
	}

	free(str);
	reset_loader();
}
