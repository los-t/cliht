#include <stdio.h>
#include <string.h>

#include "cfg.h"
#include "error.h"
#include "http.h"
#include "option.h"
#include "net.h"
#include "url.h"

int main(int argc, char *argv[]) {

	char* msg = NULL;
	char* hdr = NULL;
	char* body = NULL;
	FILE* file = NULL;
	char* filepath = NULL;

	int sock = 0;
	
	
	Configuration cfg;
	Url url;

	ERROR_CODE err = ERR_NONE;

	memset(&cfg, 0, sizeof(Configuration));
	memset(&url, 0, sizeof(Url));

  err = cfg_init_from_cli(argc, argv, &cfg);
	if (err == ERR_CFG_NOARGS || err == ERR_CFG_HELPREQUEST) {
		print_usage();
		return 0;
	}

	err = url_init_from_str(&url, cfg.url);
	if (err != ERR_NONE) {
		err_print(err);
		exit(EXIT_FAILURE);
	}

	err = http_build_request(&msg, &url);
	if (err != ERR_NONE) {
		err_print(err);
		exit(EXIT_FAILURE);
	}

	err = net_connect(&sock, url.host, url.port);
	if (err != ERR_NONE) {
		err_print(err);
		exit(EXIT_FAILURE);
	}

	err = net_send(sock, msg);
	if (err != ERR_NONE) {
		err_print(err);
		exit(EXIT_FAILURE);
	}

	/* err = net_get_until(sock, &hdr, "\r\n\r\n");
	if (err != ERR_NONE) {
		err_print(err);
		exit(EXIT_FAILURE);
	} */

	printf("Received data from %s\nHEAD: [%s]\n", url.host, hdr);

	if (asprintf(&filepath, "%s/%s", cfg.lpath, url.host) < 0) {
		err_print(ERR_MEMFAIL);
		exit(EXIT_FAILURE);
	}

	printf("Saving to %s\n", filepath);

	file = fopen(filepath, "w");
	if (!file) {
		err_print(ERR_FILE_OPENFAIL);
		exit(EXIT_FAILURE);
	}

	/*err = net_get_until(sock, &body, "\r\n\r\n");
	if (err != ERR_NONE) {
		err_print(err);
		exit(EXIT_FAILURE);
	}

	if (fputs(body, file) == EOF) {
		err_print(ERR_FILE_WRITEFAIL);
		exit(EXIT_FAILURE);
	} */

	/* Clean up */
	fclose(file);
	free(filepath);
	net_free(sock);
	if (body) free(body); 
	if (hdr) free(hdr); 
	if (msg) free(msg); 
	url_free(&url);
	cfg_free(&cfg);
	
	return 0;
}
