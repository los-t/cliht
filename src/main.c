#include <stdio.h>
#include <string.h>

#include "cfg.h"
#include "const.h"
#include "error.h"
#include "http.h"
#include "option.h"
#include "url.h"

int main(int argc, char *argv[]) {

	char* msg = NULL;
	
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
		printf("URL could not be parsed [%x]\n", err);
		return 1;
	}

	err = http_build_request(&msg, &url);
	if (err != ERR_NONE) {
		printf("Could not build HTTP request [%x]\n", err);
		return 1;
	}

	printf("Msg:[\n%s]\n", msg);
	/* http_save_data(url, cfg.lpath) */

	/* Clean up */
	url_free(&url);
	cfg_free(&cfg);
	
	return 0;
}
