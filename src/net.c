#include <netdb.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "net.h"

const size_t NET_CHUNKSIZE = 1024;

typedef struct addrinfo ADDRINFO;

ERROR_CODE net_connect(int* sock, const char* host, const long port) {
	ADDRINFO *addr, *p;
	char service[64];
	int res = 0;
	
	snprintf(service, 64*sizeof(char), "%d", port);
	res = getaddrinfo(host, service, NULL, &addr);
	if (res != 0) {
		return ERR_NET_ADDRFAIL;
	}

	for(p = addr; p != NULL; p->ai_next) {
		if ((*sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
			continue;

		if (connect(*sock, p->ai_addr, p->ai_addrlen) == -1)
			continue;

		break;
	}
	
	if (!p) {
		*sock = 0;
		return ERR_NET_CONNFAIL;
	}

	freeaddrinfo(addr);

	return ERR_NONE;
}

ERROR_CODE net_send(int sock, const char* msg) {
	size_t nsent = 0;
	size_t size = strlen(msg) * sizeof(msg);

	if (!sock) {
		return ERR_NET_SOCKNOTVALID;
	}

	nsent = send(sock, msg, size, 0);
	if(nsent < 0) {
		return ERR_NET_SENDFAIL;
	}

	return ERR_NONE;
}

ERROR_CODE net_get_until(int sock, char** buf, const char* terminator) {
	char tmp;
	size_t count = 0;
	size_t nbyte = 0;
	size_t bufsize = NET_CHUNKSIZE;

	*buf = (char*)malloc(NET_CHUNKSIZE);

	while(net_recv_byte(sock, &tmp) == ERR_NONE) {
		if (!terminator[count])
			break;

		if (tmp == terminator[count])
			count++;
		else
			count = 0;

		nbyte++;
		if (nbyte == bufsize) {
			/* enlarge the buffer */
			bufsize += NET_CHUNKSIZE;
			*buf = (char*)realloc(*buf, bufsize);
		}

		(*buf)[nbyte] = tmp;
	}
	(*buf)[nbyte] = '\0';

	return ERR_NONE;
}


ERROR_CODE net_free(int sock) {
	shutdown(sock, SHUT_RDWR);
	
	return ERR_NONE;
}

ERROR_CODE net_recv_byte(int sock, char* byte) {
	size_t n = 0;

	n = recv(sock, byte, sizeof(*byte), 0);

	if (n == 0)
		return ERR_NET_CONNCLOSED;

	if (n < 0)
		return ERR_NET_RECVFAIL;

	return ERR_NONE;
}
