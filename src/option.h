#ifndef CLIHT_OPTION_H
#define CLIHT_OPTION_H

#include "error.h"

typedef struct OptStruct {
	char *lname;  /* long option */
	char *sname;  /* short option */
	char *vname;  /* value name */
} Option;

extern const Option OPT_HELP;
extern const Option OPT_OUTPATH;
 
unsigned int arg_is(const char* /*argument*/, const Option /*option*/);
unsigned int arg_is_short(const char* /*argument*/, const Option /*option*/);
unsigned int arg_is_long(const char* /*argument*/, const Option /*option*/);

ERROR_CODE print_option(const Option /*option*/, const char* /*description*/);

void print_usage();

#endif // CLIHT_OPTION_H
