// Sachin Shah
// April 2, 2020
// Regex helper

#ifndef _RE_H
#define _RE_H

#include <regex.h>
#include <stdint.h>

void compile_regex(regex_t *re, const char *pattern);
int8_t match(regex_t *re, regmatch_t *groups, size_t n_groups, char *str);

regmatch_t *allocate_groups(size_t groups);

#endif