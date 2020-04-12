// Sachin Shah
// April 11, 2020
// Dynamic library compiler & loader

#include "dylib.h"
#include "flags.h"
#include "re.h"
#include "utils.h"

#include <dlfcn.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

void init_loader(void)
{
	FILE *header;
	compile_regex(&var_dec_re, var_dec);
	compile_regex(&var_int_re, var_int);

	compile_regex(&fun_dec_re, fun_dec);
	compile_regex(&fun_int_re, fun_int);

	// Clear previous libraries
	system("rm -f dls/*");

	// Add the inital empty library
	system("gcc -shared -fpic -o dls/libsempty.so empty.c");

	header = fopen("scope.h", "w");
	check_null(header, "failed to open scope file");
	fprintf(header, "#include <stdio.h>\n#include <stdlib.h>\n\n");

	fclose(header);
}

void reset_loader(void)
{
	regfree(&var_dec_re);
	regfree(&var_int_re);
	regfree(&fun_dec_re);
	regfree(&fun_int_re);

	add_to_scope("x", "");

	system("rm -f dls/*");
	system("rm -f scope.h");

	system("gcc -shared -fpic -o dls/libsempty.so empty.c");
}

uint32_t num_length(uint32_t num)
{
	if (num == 0) return 1;
	return floor(log10(num)) + 1;
}

flag_t eval(char *content, uint32_t num)
{
	char *source, *library, *compile;
	void *ctr = NULL;
	uint32_t len = num_length(num);
	function_t foo = NULL;
	int8_t delete_source;
	flag_t ret = ERROR;

	// Create source .c file name
	source = (char *) calloc(sizeof(char), SRC_N + len + 3);
	check_null(source, "calloc failed to find space for source name");
	sprintf(source, "%s%d.c", src, num);

	// Create library .so file name
	library = (char *) calloc(sizeof(char), DLS_N + len + 4);
	check_null(library, "calloc failed to find space for lib name");
	sprintf(library, "%s%d.so", dls, num);

	// Create compile command
	compile = (char *) calloc(sizeof(char), GCC_N + LIB_N + (len * 2) + SRC_N + DLS_N + PIPE_N + 10);
	check_null(compile, "calloc failed to find space for compile name");
	sprintf(compile, "%s%s %s %s %s", gcc, library, source, lib, pipe);

	delete_source = write_lib(content, source);

	if (system(compile) == 0)
	{
		ctr = dlopen(library, RTLD_NOW | RTLD_GLOBAL);
		if (ctr != NULL)
		{
			foo = (function_t) dlsym(ctr, temp_function);
			if (foo != NULL) foo();
		}
		ret = SUCCESS;
	}
	else if ((delete_source & 2) == 2) add_to_scope("", "");

	if ((delete_source & 1) == 1)
	{
		sprintf(compile, "rm -f %s", source);
		system(compile);
		sprintf(compile, "rm -f %s", library);
		system(compile);
		if (ctr != NULL) dlclose(ctr);
		if (ret == SUCCESS) ret = NO_SOURCE;
	}

	free(library);
	free(compile);
	free(source);

	return ret;
}

void add_to_scope(char *declaration, char* prefix)
{
	static long int last_entry = 0;
	static long int offset = 0;
	long int temp;
	FILE *fp;
	fpos_t pos;

	fp = fopen("scope.h", "r+");
	check_null(fp, "failed to open scope header");

	fseek(fp, offset, SEEK_END);

	if (declaration[0] == '\0' && prefix[0] == '\0')
	{
		temp = ftell(fp);
		offset = last_entry - temp;
		fseek(fp, last_entry, SEEK_SET);
		while (temp-- > last_entry) fprintf(fp, " ");
	}
	else
	{
		last_entry = ftell(fp);
		offset = 0;

		fprintf(fp, "%s%s\n", prefix, declaration);
	}

	fclose(fp);
}

void write_null(FILE *fp)
{
	fprintf(fp, "void %s(void){ /* do nothing */ }\n", temp_function);
}

int8_t write_lib(char *content, char *name)
{
	FILE *fp;
	int8_t ret = 0;
	char *kill_char = NULL;

	fp = fopen(name, "w");
	check_null(fp, "failed to open source file");

	fprintf(fp, "#include \"../scope.h\"\n\n");

	if (content[0] == '#')
	{
		add_to_scope(content, "");
		ret = 3;
	}
	else if (match(&var_dec_re, NULL, 0, content) == 1)
	{
		add_to_scope(content, "extern ");
		fprintf(fp, "%s\n", content);
		write_null(fp);
		ret = 2;
	}
	else if (match(&var_int_re, NULL, 0, content) == 1)
	{
		fprintf(fp, "%s\n", content);
		write_null(fp);

		kill_char = strpbrk(content, "=");
		kill_char[0] = ';';
		kill_char[1] = '\0';

		add_to_scope(content, "extern ");
		ret = 2;
	}
	else if (match(&fun_int_re, NULL, 0, content) == 1)
	{
		fprintf(fp, "%s\n", content);
		write_null(fp);

		kill_char = strpbrk(content, "{");
		kill_char[0] = ';';
		kill_char[1] = '\0';

		add_to_scope(content, "");
		ret = 2;
	}
	else if(match(&fun_dec_re, NULL, 0, content) == 1)
	{
		add_to_scope(content, "");
		write_null(fp);
		ret = 3;
	}
	else fprintf(fp, "void %s(void)\n{\n%s\n}\n", temp_function, content);

	fclose(fp);

	return ret;
}
