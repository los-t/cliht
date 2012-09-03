#include <stdlib.h>
#include <memory.h>
#include <string.h>

#include "option.h"

#include "cfg.h"

ERROR_CODE cfg_init_from_cli(int argc, char **argv, Configuration *cfg) {
	char *equals_char = NULL;
	size_t arglen = 0;
	int i = 0;
	
	/* Clear the configuration first */
	cfg_free(cfg);

	if (argc == 1)
		return ERR_CFG_NOARGS;

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
				set_value(&cfg->lpath, argv[i] + strlen(OPT_OUTPATH.sname)*sizeof(*OPT_OUTPATH.sname));
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

	return ERR_NONE;
}

ERROR_CODE set_value(char** opt, char* arg) {
	size_t dummylen;
	size_t dummysize;

	dummylen = strlen(arg);
	dummysize = dummylen * sizeof(*arg);

	*opt = (char*)malloc(dummysize);
	memset(*opt, 0, dummysize);

	strncpy(*opt, arg, dummylen);
	
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
