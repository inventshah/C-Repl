#include "dylib.h"
#include "read.h"
#include "flags.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	char *str;
	uint32_t num = 0;
	init_loader();
	flag_t code;

	while (1)
	{
		str = read();
		if (strcmp(str, "quit;") == 0) break;
		if (strcmp(str, "clear;") == 0)
		{
			reset_loader();
			continue;
		}

		code = eval(str, num++);

		switch (code)
		{
			case ERROR: printf("syntax error\n");
			case NO_SOURCE: num--;
			default: break;
		}

		free(str);
	}

	free(str);
	clear_loader();
	free_regex();
}
