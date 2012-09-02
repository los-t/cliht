#include <stdio.h>
#include <string.h>

#include "cfg.h"
#include "const.h"
#include "error.h"
#include "option.h"
#include "url.h"

int main(int argc, char *argv[]) {
	
	Configuration cfg;
	Url url;

	ERROR_CODE err = ERR_NONE;

	memset(&cfg, 0, sizeof(Configuration));
	memset(&url, 0, sizeof(Url));

  err = cfg_init_from_cli(argc, argv, &cfg);
	if (err == ERR_CFG_NOARGS || err == ERR_CFG_HELPREQUEST) {
		print_usage();
		return 0;
	}

	err = url_init_from_str(&url, cfg.url);
	if (err != ERR_NONE) {
		printf("URL could not be parsed [%d]\n", err);
		return 1;
	}

	/* http_save_data(url, cfg.lpath) */

	/* Clean up */
	url_free(&url);
	cfg_free(&cfg);
	
	return 0;
}
