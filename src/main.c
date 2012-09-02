#include <stdio.h>
#include <string.h>

#include "const.h"
#include "cfg.h"
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

	printf("CFG::url [%s]\n", (cfg.url) ? cfg.url : "NA");
	printf("CFG::path[%s]\n", (cfg.lpath) ? cfg.lpath : "NA");

	err = url_init_from_str(&url, cfg.url);
	if (err != ERR_NONE) {
		printf("URL could not be parsed [%d]\n", err);
		return 1;
	}
	/*TEST*/

	printf("URL::proto [%s]\n", url.proto);
	printf("URL::host  [%s]\n", url.host);
	printf("URL::port  [%d]\n", url.port);
	printf("URL::path  [%s]\n", url.path);

	/* Parse and verify the URL */
	/* Check that we can access the OUTPUT path, create if necessary */
	/* Try to connect to HOST on a given PORT */
	/* Prepare GET request */
	/* Send GET request */
	/* Start Readng response from HOST dumping to immediately */
	/* Close the handles and clean up */
	url_free(&url);
	cfg_free(&cfg);
	
	return 0;
}
