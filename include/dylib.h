// Sachin Shah
// April 11, 2020
// Dynamic library compiler & loader

#ifndef _DYLIB_H
#define _DYLIB_H

#include "re.h"
#include "flags.h"

#include <stdint.h>

#define SRC_N 4
#define DLS_N 10
#define GCC_N 21
#define LIB_N 8
#define PIPE_N 24

const char *src = "dls/dl";
const char *dls = "dls/libsdl";
const char *gcc = "gcc -shared -fpic -o ";
const char *lib = "dls/*.so";

const char *pipe = "> /dev/null 2> /dev/null";

const char *temp_function = "tmpfnctn";

const char *var_dec = "^[a-zA-Z_ *]+[ \t\n*]+([a-zA-Z_0-9]|\\[|\\])+;";
const char *var_int = "^[a-zA-Z_ *]+[ \t\n*]+([a-zA-Z_0-9]|\\[|\\])+([ \t\n]*[=].*)?;";

const char *fun_dec = "^[a-zA-Z_ *]+[ \t\n*]+([a-zA-Z_0-9]|\\[|\\])+[ \t\n]*\\([^{]*\\)[ \t\n]*;";
const char *fun_int = "^[a-zA-Z_ *]+[ \t\n*]+([a-zA-Z_0-9]|\\[|\\])+[ \t\n]*\\((.|\n)*\\)[ \t\n]*\\{";

regex_t var_dec_re;
regex_t var_int_re;
regex_t fun_dec_re;
regex_t fun_int_re;

typedef void (*function_t)(void);

void init_loader(void);
void reset_loader(void);

uint32_t num_length(uint32_t num);

void add_to_scope(char *declaration, char *prefix);

flag_t eval(char *function, uint32_t num);
int8_t write_lib(char *content, char *name);

#endif