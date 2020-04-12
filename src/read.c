// Sachin Shah
// April 11, 2020
// Read code lines from terminal

#include "read.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

int32_t count_brackets(char *string, size_t length)
{
	uint32_t i;
	int32_t count = 0;
	for (i = 0; i < length; i++)
	{
		if (string[i] == '{') count++;
		else if (string[i] == '}') count--;
	}

	return count;
}

char *read(void)
{
	char *buffer, *root;
	uint32_t size = BUFFER_SIZE, total_brackets = 0;
	uint32_t current_size = 0;
	size_t length;

	root = (char *) calloc(sizeof(char), size);
	check_null(root, "failed to find space for buffer");
	buffer = root;

	printf(">>> ");
	while (1)
	{
		fgets(buffer, size, stdin);
		chomp(buffer);
		length = strlen(buffer);

		total_brackets += count_brackets(buffer, length);

		if (total_brackets == 0 && buffer[length - 1] == ';') break;
		if (total_brackets == 0 && buffer != root) break;

		buffer += length;
		current_size += length;

		if (current_size * 1.25 > size)
		{
			size *= 1.25;
			root = (char *) realloc(root, size);
		}
		printf("... ");
	}

	return root;
}
