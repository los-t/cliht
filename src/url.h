#ifndef CLIHT_URL_H_
#define CLIHT_URL_H_

#include <stdlib.h>

#include "error.h"

typedef struct UrlStruct {
	char *proto;
	char *host;
	long port;
	char *path;
} Url;

ERROR_CODE url_create(Url** /*URL*/, const char* /*string*/);
ERROR_CODE url_free(Url** /*URL*/);

#endif // CLIHT_URL_H_
