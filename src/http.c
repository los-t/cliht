#define _GNU_SOURCE
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
	if (*dst)
		free(*dst);

	if (asprintf(dst, "%s%s%s%s%s", method, HTTP_SP, path, HTTP_SP, HTTP_VERSION) < 0) {
		*dst = NULL;
		return ERR_HTTP_BLDFAIL_LINE;
	}

	return ERR_NONE;
}

ERROR_CODE http_build_request_header(char** dst, const char* hdr, const char* val) {
	if (*dst)
		free(dst);

	if (asprintf(dst, "%s%s%s", hdr, HTTP_HDR_SP, val) < 0) {
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
	char dummy[1024];

	ERROR_CODE err = ERR_NONE;

	err = http_build_request_line(&line, HTTP_MTD_GET, url->path);

	snprintf(dummy, 1024, "%s:%ld", url->host, url->port);
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

	if (asprintf(dst, "%s\n%s\n%s\n%s\n\n\n", line, hdr_host, hdr_agent, hdr_conn) < 0) {
		*dst = NULL;
		return ERR_HTTP_BLDFAIL_RQST;
	}

	free(hdr_conn);
	free(hdr_agent);
	free(hdr_host);
	free(line);

	return ERR_NONE;
}

