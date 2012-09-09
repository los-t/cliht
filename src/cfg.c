#define POSIX_C_SOURCE 200809L
#define _GNU_SOURCE

#include <assert.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "option.h"

#include "cfg.h"

const char* DFLT_OUTPUTPATH = ".";

ERROR_CODE cfg_init_from_cli(int argc, char **argv, Configuration *cfg) {
	char *equals_char = NULL;
	size_t arglen = 0;
	int i = 0;
	
	/* Clear the configuration first */
	cfg_free(cfg);

	assert(argc > 1);

	for(i = 1; i < argc; i++) {
		if (arg_is(argv[i], OPT_HELP)) {
			return ERR_CFG_HELPREQUEST;
		}

		equals_char = strchr(argv[i], '=');

		if (arg_is(argv[i], OPT_OUTPATH)) {
			if ( equals_char ) {
				/* option is '--opt=value' or '-o=value' */
				set_value(&cfg->lpath, ++equals_char);
			} else if (arg_is_short(argv[i], OPT_OUTPATH) &&
			           arglen > strlen(OPT_OUTPATH.sname)) {
				/* option is '-ovalue' */
				set_value(&cfg->lpath, argv[i] + strlen(OPT_OUTPATH.sname));
			} else {
				/* option is '-o value' or '--opt value' */
				i++;

				if (i == argc)
					return ERR_CFG_NOARGS;

				set_value(&cfg->lpath, argv[i]);
			}
		} else {
			/* We have a URI */
			set_value(&cfg->url, argv[i]);
		}
	}

	if (!cfg->lpath)
		set_value(&cfg->lpath, DFLT_OUTPUTPATH);

	return ERR_NONE;
}

ERROR_CODE set_value(char** opt, const char* arg) {
	*opt = strdup(arg);

	/* TODO: should we check the postcondition here? */
	assert(*opt != NULL);
	
	return ERR_NONE;
}

ERROR_CODE cfg_free(Configuration *cfg) {
	if (!cfg)
		return ERR_NONE;
	
	if (cfg->url) {
		free(cfg->url);
		cfg->url = NULL;
	}

	if (cfg->lpath) {
		free(cfg->lpath);
		cfg->lpath = NULL;
	}

	return ERR_NONE;
}
