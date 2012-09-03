#include <stdio.h>
#include <string.h>

#include "http.h"

const char* HTTP_SP = " ";
const char* HTTP_VERSION = "HTTP/1.0";
const char* HTTP_USERAGENT = "cliht/0.1";

const char* HTTP_MTD_GET = "GET";

const char* HTTP_HDR_SP = ": ";
const char* HTTP_HDR_HOST = "Host";
const char* HTTP_HDR_AGENT = "User-agent";
const char* HTTP_HDR_CONN = "Connection";

ERROR_CODE http_build_request_line(char** dst, const char* method, const char* path) {
	size_t len = strlen(method) + strlen(HTTP_SP) + strlen(path) + strlen(HTTP_SP) + strlen(HTTP_VERSION)
	           + 1; /* for NULL terminator */
	int sz = 0;

	if(*dst)
		free(*dst);

	*dst = (char*)malloc(len * sizeof(char));
	if (!*dst) {
		return ERR_MEMFAIL;
	}

	snprintf(*dst, len, "%s%s%s%s%s", method, HTTP_SP, path, HTTP_SP, HTTP_VERSION);
	if (sz < 0) {
		free(*dst);
		*dst = NULL;
		return ERR_HTTP_BLDFAIL_LINE;
	}

	return ERR_NONE;
}

ERROR_CODE http_build_request_header(char** dst, const char* hdr, const char* val) {
	size_t len = strlen(hdr) + strlen(HTTP_HDR_SP) + strlen(val)
	           + 1; /* for NULL terminator */
	int sz = 0;

	if(*dst)
		free(dst);

	*dst = (char*)malloc(len * sizeof(char));
	if (!*dst) {
		return ERR_MEMFAIL;
	}
	
	sz = snprintf(*dst, len, "%s%s%s", hdr, HTTP_HDR_SP, val);
	if (sz < 0) {
		free(*dst);
		*dst = NULL;
		return ERR_HTTP_BLDFAIL_HDR;
	}

	return ERR_NONE;
}

ERROR_CODE http_build_request(char** dst, const Url* url) {
	char* line = NULL;
	char* hdr_host = NULL;
	char* hdr_agent = NULL;
	char* hdr_conn = NULL;
	size_t msglen = 0;
	char dummy[1024];
	int sz = 0;

	ERROR_CODE err = ERR_NONE;

	err = http_build_request_line(&line, HTTP_MTD_GET, url->path);

	snprintf(dummy, 1024, "%s:%d", url->host, url->port);
	err = http_build_request_header(&hdr_host, HTTP_HDR_HOST, dummy); 
	if (err != ERR_NONE) {
		return err;
	}

	err = http_build_request_header(&hdr_agent, HTTP_HDR_AGENT, HTTP_USERAGENT);
	if (err != ERR_NONE) {
		return err;
	}

	err = http_build_request_header(&hdr_conn, HTTP_HDR_CONN, "close");
	if (err != ERR_NONE) {
		return err;
	}

	msglen = strlen(line) + 1
	       + strlen(hdr_host) + 1
	       + strlen(hdr_agent) + 1
	       + strlen(hdr_conn) + 1
	       + 2; /* Empty line */

	*dst = (char*)malloc(msglen*sizeof(char));
	if (!*dst) {
		return ERR_MEMFAIL;
	}

	snprintf(*dst, msglen, "%s\n%s\n%s\n%s\n\n\n", line, hdr_host, hdr_agent, hdr_conn);
	if (sz < 0) {
		free(*dst);
		*dst = NULL;
		return ERR_HTTP_BLDFAIL_RQST;
	}

	free(hdr_conn);
	free(hdr_agent);
	free(hdr_host);
	free(line);

	return ERR_NONE;
}

