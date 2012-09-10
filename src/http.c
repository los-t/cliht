#define _GNU_SOURCE
#include <assert.h>
#include <stdio.h> 
#include <string.h>

#include "fs.h" 
#include "net.h"

#include "http.h"

const char* HTTP_SP = " ";
const char* HTTP_VERSION = "HTTP/1.0";
const char* HTTP_USERAGENT = "cliht/0.1";

const char* HTTP_MTD_GET = "GET";

const char* HTTP_HDR_SP = ": ";
const char* HTTP_HDR_HOST = "Host";
const char* HTTP_HDR_AGENT = "User-agent";
const char* HTTP_HDR_CONN = "Connection";

const char* HTTP_RESPHDR_SP = "\r\n\r\n";

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
		*dst = NULL; return ERR_HTTP_BLDFAIL_HDR; }

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

ERROR_CODE http_header_process(char* hdr) {
	/* for now just print to stdout */
	printf("Server reponse header:\n%s\n", hdr);
	return ERR_NONE;
}

/* TODO: Review the algorithm if a more generic approach to special case is possible */
const char* http_header_helper(const char* data, size_t* datasize) {
	static char* hdr = NULL;
	static size_t hdrlen = 0;
	char* hdr_end = NULL;
	size_t size = *datasize;

	assert(data != NULL);

	/* Special case when a delimiter was split in two chunks, avoiding reallocation */
	if (hdr && 
			hdr[hdrlen-1] == HTTP_RESPHDR_SP[0] &&
			data[0] == HTTP_RESPHDR_SP[1]) {
		http_header_process(hdr);
		free(hdr);

		(*datasize)--;
		return data + 1;
	}
	
	hdr_end = strstr(data, HTTP_RESPHDR_SP);
	if(hdr_end)
		/* Take care of a special case when header was already processed but delimiter arrived in next chunk */
		size = (hdr_end != data) ? (hdr_end - data) : 0;

	if (size) {
		hdr = realloc(hdr, hdrlen + size);
		assert(hdr != NULL);
		strncat(hdr, data, size);
		hdrlen = hdrlen + size;
	}

	if(hdr_end) {
		http_header_process(hdr);
		free(hdr);

		*datasize = size;
		return hdr_end + strlen(HTTP_RESPHDR_SP);
	}

	return NULL;
}

ERROR_CODE http_save_helper(NET_HANDLER_OBJ file, const char* data, const size_t size) {
	File *strm = (File*)file;
	static unsigned int am_expecting_hdr = 1;
	const char* data_to_dump = data;
	size_t tmpsize = size;

	if (am_expecting_hdr) {
		if (!(data_to_dump = http_header_helper(data, &tmpsize)))
			return ERR_NONE;

		am_expecting_hdr = 0;
	}

	if(!tmpsize)
		/* Not writing anything - all went to header */
		return ERR_NONE;

	return fs_write(strm, data_to_dump, tmpsize);
}

ERROR_CODE http_get(const Url* url, const char* path) {
	char *request = NULL;
	int sock = 0;
	File *file = NULL;
	ERROR_CODE err = ERR_NONE;	

	err = net_connect(&sock, url->host, url->port);
	if (err != ERR_NONE)
		return err;

	err = http_build_request(&request, url);
	if (err != ERR_NONE)
		return err;
		
	err = net_send(sock, request);
	if (err != ERR_NONE)
		return err;

	err = fs_create(&file, path);
	if (err != ERR_NONE)
		return err;

	err = net_get(sock, &http_save_helper, file);
	if (err != ERR_NONE)
		return err;

	fs_free(&file);
	net_free(sock);
	free(request);

	return ERR_NONE;
}
