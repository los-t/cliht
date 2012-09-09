#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>

#include "cfg.h"
#include "error.h"
#include "http.h"
#include "option.h"
#include "net.h"
#include "url.h"

int main(int argc, char *argv[]) {
	char* filepath = NULL;
	
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
		err_print(err);
		exit(EXIT_FAILURE);
	}

	if (asprintf(&filepath, "%s/%s", cfg.lpath, url.host) < 0) {
		err_print(ERR_MEMFAIL);
		exit(EXIT_FAILURE);
	}

	printf("Saving to %s\n", filepath);

	err = http_get(&url, filepath);
	if (err != ERR_NONE) {
		err_print(err);
		exit(EXIT_FAILURE);
	}

	free(filepath);
	url_free(&url);
	cfg_free(&cfg);
	
	return 0;
}
