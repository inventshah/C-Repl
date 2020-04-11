// Sachin Shah
// April 2, 2020

#include "re.h"
#include "utils.h"

#include <regex.h>
#include <stdint.h>

#include <stdlib.h>

void compile_regex(regex_t *re, const char *pattern)
{
	int code = regcomp(re, pattern, REG_EXTENDED | REG_ENHANCED);
	if (code) error("Could not compile regex.");
}

int8_t match(regex_t *re, regmatch_t *groups, size_t n_groups, char *str)
{
	int ret = regexec(re, str, n_groups, groups, 0);

	if (!ret) return 1;
	else if (ret == REG_NOMATCH) return 0;
	else regerror(ret, re, str, sizeof(str));
	return -1;
}

regmatch_t *allocate_groups(size_t groups)
{
	regmatch_t *group = (regmatch_t *) calloc(sizeof(regmatch_t), groups);
	check_null(group, "calloc failed to find space for match groups.");

	return group;
}