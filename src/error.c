#include <stdio.h>
#include "error.h"

void err_print(ERROR_CODE err) {
	switch(err) {
		case ERR_NONE: perror("No error.\n"); break;
		case ERR_MEMFAIL: perror("Memory allocation failed.\n"); break;
		case ERR_CFG_NOARGS: perror("Not enough arguments.\n"); break;
		case ERR_CFG_HELPREQUEST: perror("Help was requested.\n"); break;
		case ERR_URL_CANNOTPARSE: perror("URL cannot be parsed.\n"); break;
		case ERR_URL_NOTENOUGHDATA: perror("URL malformed.\n"); break;
		case ERR_HTTP_BLDFAIL_HDR: perror("Could not build HTTP header.\n"); break;
		case ERR_HTTP_BLDFAIL_LINE: perror("Could not build HTTP request-line.\n"); break;
		case ERR_HTTP_BLDFAIL_RQST: perror("Could not build HTTP request"); break;
		case ERR_NET_ADDRFAIL: perror("Address resolution failed.\n"); break;
		case ERR_NET_CONNFAIL: perror("Connection attempt failed.\n"); break;
		case ERR_NET_SOCKNOTVALID: perror("Socket is not valid.\n"); break;
		case ERR_NET_SENDFAIL: perror("Could not send message.\n"); break;
		case ERR_NET_RECVFAIL: perror("Could not receive data from network.\n"); break;
		case ERR_NET_CONNCLOSED: perror("Connection was closed."); break;
		case ERR_FILE_OPENFAIL: perror("Could not open file."); break;
		case ERR_FILE_WRITEFAIL: perror("Could not write to file."); break;
	}
}
