#ifndef CLIHT_HTTP_H_
#define CLIHT_HTTP_H_

#include "error.h"
#include "url.h"

extern const char* HTTP_SP;
extern const char* HTTP_VERSION;
extern const char* HTTP_USERAGENT;

extern const char* HTTP_MTD_GET;

extern const char* HTTP_HDR_SP;
extern const char* HTTP_HDR_HOST;
extern const char* HTTP_HDR_AGENT;
extern const char* HTTP_HDR_CONN;

ERROR_CODE http_build_request_line(char** dst, const char* method, const char* path);
ERROR_CODE http_build_request_header(char** dst, const char* hdr, const char* val);
ERROR_CODE http_build_request(char** dst, const Url* url);

#endif // CLIHT_HTTP_H_
