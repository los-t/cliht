#include <stdio.h>
#include <string.h>

#include "cfg.h"
#include "const.h"
#include "error.h"
#include "http.h"
#include "option.h"
#include "url.h"

int main(int argc, char *argv[]) {

	char* msg = NULL;
	char* hdr = NULL;
	char* body = NULL;
	FILE* file = NULL;
	char* filepath = NULL;
	size_t fplen = 0;

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
		printf("URL could not be parsed [%x]\n", err);
		return 1;
	}

	err = http_build_request(&msg, &url);
	if (err != ERR_NONE) {
		printf("Could not build HTTP request [%x]\n", err);
		return 1;
	}

	err = net_connect(&sock, url.host, url.port);
	if (err != ERR_NONE) {
		return err;
	}

	err = net_send(sock, msg);
	if (err != ERR_NONE) {
		return err;
	}

	err = net_get_until(sock, &hdr, "\r\n\r\n");
	if (err != ERR_NONE) {
		return err;
	}

	printf("Received data from %s\nHEAD: [%s]\n", url.host, hdr);

	fplen = strlen(cfg.lpath) + strlen(url.host) + 2;
	filepath = (char*)malloc(fplen * sizeof(char));
	snprintf(filepath, fplen, "%s/%s", cfg.lpath, url.host);
	printf("Saving to %s\n", filepath);

	file = fopen(filepath, "w");

	err = net_get_until(sock, &body, "\r\n\r\n");
	if (err != ERR_NONE) {
		return err;
	}
	fputs(body, file);

	/* Clean up */
	close(file);
	free(filepath);
	net_free(sock);
	if (body) free(body); 
	if (hdr) free(hdr); 
	if (msg) free(msg); 
	url_free(&url);
	cfg_free(&cfg);
	
	return 0;
}
