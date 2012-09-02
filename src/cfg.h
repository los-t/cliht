#ifndef CLIHT_CFG_H_
#define CLIHT_CFG_H_

#include "error.h"

typedef struct CfgStruct {
	char *url;
	char *lpath;
} Configuration;

ERROR_CODE cfg_init_from_cli(int /*argc*/, char ** /*argv*/, Configuration * /*cfg*/);
ERROR_CODE cfg_free(Configuration * /*cfg*/);

ERROR_CODE set_value(char ** /*option*/, char * /*arg*/);

#endif // CLIHT_CFG_H_
