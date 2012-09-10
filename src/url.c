#include <stdlib.h>
#include <string.h>

#include "utl.h"

#include "url.h"

const long URL_DFLT_PORT = 80;
const char* URL_DFLT_PROTO = "http";
const char* URL_DFLT_PATH = "/";

const char* URL_DELIM_PROTO = "://";
const char* URL_DELIM_PORT = ":";
const char* URL_DELIM_ROOT = "/";

ERROR_CODE url_create(Url** url, const char* strurl) {
	const char* pos = strurl;
	const char* npos = strurl;

	const char* host_pos = NULL;
	const char* host_end = NULL;
	const char* port_pos = NULL;
	const char* root_pos = NULL;

	if(!strurl) {
		return ERR_URL_NOTENOUGHDATA;
	}

	*url = (Url*)malloc(sizeof(Url));

	npos = strstr(pos, URL_DELIM_PROTO);
	if (npos) {
		/* we have a protocol */
		(*url)->proto = strndup(pos, npos - pos);
	} else {
		/* set protocol to default */
		(*url)->proto = strdup(URL_DFLT_PROTO);
	}
	host_pos = npos ? npos + strlen(URL_DELIM_PROTO) : pos;

	npos = strstr(host_pos, URL_DELIM_PORT);
	if (npos) {
		host_end = npos;
		port_pos = npos + strlen(URL_DELIM_PORT);
	}

	npos = strstr(port_pos?port_pos:host_pos, URL_DELIM_ROOT);
	if (npos) {
		if (!host_end)
			host_end = npos;
		root_pos = npos;
	}

	npos = port_pos;
	if (port_pos) {
		(*url)->port = strtol(port_pos, &npos, 10);
	}
	if (!port_pos || npos == port_pos) {
		(*url)->port = URL_DFLT_PORT;
	}

	if (root_pos) {
		(*url)->path = strdup(root_pos);
	} else {
		(*url)->path = strdup(URL_DFLT_PATH);
	}

	if (!host_end) {
		/* we don't have a port or a path in source string, means all we have is a host */
		(*url)->host = strdup(host_pos);
	} else {
		(*url)->host = strndup(host_pos, host_end - host_pos);
	}

	printf("DBG: HOST  [%s]\n", (*url)->host);
	printf("DBG: PORT  [%ld]\n", (*url)->port);
	printf("DBG: PROTO [%s]\n", (*url)->proto);
	printf("DBG: PATH  [%s]\n", (*url)->path);
	return ERR_NONE;
}

ERROR_CODE url_free(Url** url) {
	if (!url || !(*url))
		return ERR_NONE;

	if ((*url)->proto) {
		free((*url)->proto);
		(*url)->proto = NULL;
	}

	if ((*url)->host) {
		free((*url)->host);
		(*url)->host = NULL;
	}

	if ((*url)->path) {
		free((*url)->path);
		(*url)->path = NULL;
	}
	
	free(*url);
	*url = NULL;

	return ERR_NONE;
}

