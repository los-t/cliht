#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "error.h"
#include "option.h"

const Option OPT_HELP    = { .lname = "--help",   .sname = "-h", .vname = "" };
const Option OPT_OUTPATH = { .lname = "--output", .sname = "-o", .vname = "PATH" };

unsigned int arg_is(const char *arg, const Option opt) {
	return arg_is_short(arg, opt) || arg_is_long(arg, opt);
}

unsigned int arg_is_short(const char *arg, const Option opt) {
	if (!strncmp(arg, opt.sname, strlen(opt.sname)))
		return 1;

	return 0;
}

unsigned int arg_is_long(const char *arg, const Option opt) {
	if (!strncmp(arg, opt.lname, strlen(opt.lname)))
		return 1;

	return 0;
}

ERROR_CODE print_option(const Option opt, const char* desc) {
	printf("\t%s, %s\t%s\n"
	       "\t\t%s\n", opt.lname, opt.sname, opt.vname, desc);

	return ERR_NONE;
}

void print_usage() {
	printf("Usage: cliht [OPTIONS] URL\n"
				 "\n"
	       "OPTIONS are:\n");
	print_option(OPT_OUTPATH, "Path to output directory.");
	print_option(OPT_HELP, "Prints this message");
	printf("\n"
	       "URL is well-formed HTTP URL. Other protocols are not supported.\n"
	      );
}

