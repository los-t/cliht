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

ERROR_CODE url_init_from_str(Url* url, const char* str) {
	char* proto_end  = NULL;
	char* host_begin = NULL;
	char* port_begin = NULL;
	char* path_begin = NULL;
	size_t dummy = 0;

	ERROR_CODE err = ERR_NONE;

	if (!str || !strlen(str))
		return ERR_URL_NOTENOUGHDATA;

	if(url)
		url_free(url);

	err = url_alloc(url);
	if(err != ERR_NONE) {
		/* TODO: Dump message to stderr*/
		return ERR_URL_CANNOTPARSE;
	}

	err = url_extract_proto(&url->proto, str);
	if(err != ERR_NONE) {
		/* TODO: Dump message to stderr*/
		return ERR_URL_CANNOTPARSE;
	}

	err = url_extract_host(&url->host, str);
	if(err != ERR_NONE) {
		/* TODO: Dump message to stderr*/
		return ERR_URL_CANNOTPARSE;
	}

	err = url_extract_port(&url->port, str);
	if(err != ERR_NONE) {
		/* TODO: Dump message to stderr*/
		return ERR_URL_CANNOTPARSE;
	}

	err = url_extract_path(&url->path, str);
	if(err != ERR_NONE) {
		/* TODO: Dump message to stderr*/
		return ERR_URL_CANNOTPARSE;
	}
	
	return ERR_NONE;
}

ERROR_CODE url_to_str(Url* url, char* str, size_t len) {
	return ERR_NONE;
}

ERROR_CODE url_alloc(Url* url) {
	url = (Url*)malloc(sizeof(Url));

	if(!url)
		return ERR_MEMFAIL;
	
	memset(url, 0, sizeof(Url));

	return ERR_NONE;
}

ERROR_CODE url_free(Url* url) {
	if (!url)
		return ERR_NONE;

	if (url->proto) {
		free(url->proto);
		url->proto = NULL;
	}

	if (url->host) {
		free(url->host);
		url->host = NULL;
	}

	if (url->path) {
		free(url->path);
		url->path = NULL;
	}
	
	return ERR_NONE;
}

ERROR_CODE url_extract_proto(char** dst, const char* src) {
	char* proto_end = strstr(src, URL_DELIM_PROTO);
	size_t len = 0;

	if (proto_end) {
		/* Extract protocol*/
		len = (proto_end - src)/sizeof(*src);
		init_from_str(dst, src, len);
	} else {
		/* Set default protocol*/
		len = strlen(URL_DFLT_PROTO);
		init_from_str(dst, URL_DFLT_PROTO, len);
	}

	return ERR_NONE;
}

ERROR_CODE url_extract_host(char** dst, const char* src) {
	const char* host_begin = NULL;
	const char* host_end = NULL;
	size_t len = 0;

	char* proto_end = strstr(src, URL_DELIM_PROTO);
	
	if (!proto_end) {
		host_begin = src;
	} else {
		host_begin = proto_end + strlen(URL_DELIM_PROTO)*  sizeof(*URL_DELIM_PROTO);
	}
	
	/* First try to find ':' */
	host_end = strstr(host_begin, URL_DELIM_PORT);
	
	/* If port was not found try to find root */
	if (!host_end)
		host_end = strstr(host_begin, URL_DELIM_ROOT);

	if (!host_end) {
		host_end = src + strlen(src) * sizeof(src);
		if(host_end == host_begin)
			return ERR_URL_NOTENOUGHDATA;
	}

	len = (host_end - host_begin)/sizeof(*src);
	init_from_str(dst, host_begin, len);

	return ERR_NONE;
}

ERROR_CODE url_extract_port(long* dst, const char* src) {
	const char* port_begin = strstr(src, URL_DELIM_PROTO);
	char* port_end = NULL;

	if(!port_begin) {
		/* No protocol in string */
		port_begin = src;
	} else {
		/* Protocol is present, advance to not find '/' in '://' */
		port_begin += strlen(URL_DELIM_PROTO) * sizeof(*URL_DELIM_PROTO);
	}

	port_begin = strstr(port_begin, URL_DELIM_PORT);
	
	if(!port_begin) {
		/* Set default port */
		*dst = URL_DFLT_PORT;
		return ERR_NONE;
	}
	
	port_begin += strlen(URL_DELIM_ROOT) * sizeof(*URL_DELIM_ROOT);
	port_end = strstr(port_begin, URL_DELIM_ROOT);

	*dst = strtol(port_begin, &port_end, 10);
	
	return ERR_NONE;
}

ERROR_CODE url_extract_path(char** dst, const char* src) {
	const char* path_begin = strstr(src, URL_DELIM_PROTO); 
	
	if(!path_begin) {
		/* No protocol in string */
		path_begin = src;
	} else {
		/* Protocol is present, advance to not find '/' in '://' */
		path_begin += strlen(URL_DELIM_PROTO) * sizeof(*URL_DELIM_PROTO);
	}
	
	if(!*path_begin) {
		/* NULL terminator reached */
		return ERR_URL_NOTENOUGHDATA;
	}

	path_begin = strstr(path_begin, URL_DELIM_ROOT);

	if (path_begin) {
		/* At least a terminating '/' is present */
		init_from_str(dst, path_begin, strlen(path_begin)); 
	} else {
		/* Init with default */
		init_from_str(dst, URL_DFLT_PATH, strlen(URL_DFLT_PATH));
	}
	
	return ERR_NONE;
}
