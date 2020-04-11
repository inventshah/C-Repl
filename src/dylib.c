// Sachin Shah
// April 11, 2020
// Dynamic library compiler & loader

#include "dylib.h"
#include "re.h"
#include "utils.h"

#include <dlfcn.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void init_loader(void)
{
	FILE *header;
	compile_regex(&var_dec_re, var_dec);
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
	regfree(&fun_dec_re);
	regfree(&fun_int_re);

	system("rm -f dls/*");
	system("rm -f scope.h");

	system("gcc -shared -fpic -o dls/libsempty.so empty.c");
}

uint32_t num_length(uint32_t num)
{
	if (num == 0) return 1;
	return floor(log10(num)) + 1;
}

void *eval(char *content, uint32_t num)
{
	char *source, *library, *compile;
	void *ctr = NULL;
	uint32_t len = num_length(num);
	function foo = NULL;

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

	write_lib(content, source);
	
	// Compile function
	if (system(compile) == 0)
	{
		ctr = dlopen(library, RTLD_NOW | RTLD_GLOBAL);
		if (ctr != NULL)
		{
			foo = (function) dlsym(ctr, "foo");
			if (foo != NULL) foo();
		}
	}
	else
	{
		sprintf(compile, "rm -f %s", source);
		system(compile);
	}

	free(library);
	free(compile);
	free(source);

	return ctr;
}

void add_to_scope(char *declaration, char* prefix)
{
	FILE *fp;
	fpos_t pos;

	fp = fopen("scope.h", "a");
	check_null(fp, "failed to open scope header");

	fprintf(fp, "%s%s\n", prefix, declaration);

	fclose(fp);
}

void write_lib(char *content, char *name)
{
	FILE *fp;

	fp = fopen(name, "w");
	check_null(fp, "failed to open source file");

	fprintf(fp, "#include \"../scope.h\"\n\n");

	if (content[0] == '#') add_to_scope(content, "");
	else if (match(&var_dec_re, NULL, 0, content) == 1)
	{
		add_to_scope(content, "extern ");
		fprintf(fp, "%s\n", content);
	}
	else if(match(&fun_dec_re, NULL, 0, content) == 1) add_to_scope(content, "");
	else if (match(&fun_int_re, NULL, 0, content) == 1) fprintf(fp, "%s\n", content);
	else fprintf(fp, "void foo(void)\n{\n%s\n}\n", content);

	fclose(fp);
}
