#include <stdio.h>

#include "const.h"
#include "cfg.h"
#include "error.h"

int main(int argc, char *argv[]) {
	
	Configuration *cfg = NULL;

	ERROR_CODE err = cfg_init_from_cli(argc, argv, cfg);

	if (err == ERR_CFG_NOARGS || err == ERR_CFG_HELPREQUEST) {
		print_usage();
		return 0;
	}

	/* Parse and verify the URL */
	/* Check that we can access the OUTPUT path, create if necessary */
	/* Try to connect to HOST on a given PORT */
	/* Prepare GET request */
	/* Send GET request */
	/* Start Readng response from HOST dumping to immediately */
	/* Close the handles and clean up */
	cfg_free(cfg);
	
	return 0;
}
