#include <stdio.h>

#include "const.h"

void print_usage() {
	printf("Usage:\n");
	printf("cliht [OPTIONS] URL\n");
	printf("OPTIONS are:\n");

	printf("\t--output, -o\n");
	printf("\t\tPath to output directory. Default is ./\n");

	printf("\t--help, -h\n");
	printf("\t\tShow this message.\n");

	printf("URL is well-formed HTTP URL. Other protocols are not supported.\n");
}

int main(int argc, char *argv[]) {

	if (argc == 1) {
		print_usage();
		return 0;
	}
	
	return 0;
}
