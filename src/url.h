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

extern const long URL_DFLT_PORT;
extern const char* URL_DFLT_PROTO;
extern const char* URL_DFLT_PATH;
extern const char* URL_DELIM_PROTO;
extern const char* URL_DELIM_PORT;
extern const char* URL_DELIM_PATH;

ERROR_CODE url_init_from_str(Url* /*URL*/, const char* /*URL string*/);
ERROR_CODE url_conv_to_str(Url* /*URL*/, char* /*URL string*/, size_t /*Max URL length*/);
ERROR_CODE url_free(Url* /*URL*/);
ERROR_CODE url_alloc(Url* /*URL*/);

ERROR_CODE url_extract_proto(char** /*dst*/, const char* /*src*/);
ERROR_CODE url_extract_host(char** /*dst*/, const char* /*src*/);
ERROR_CODE url_extract_port(long* /*dst*/, const char* /*src*/);
ERROR_CODE url_extract_path(char** /*dst*/, const char* /*src*/);

#endif // CLIHT_URL_H_
