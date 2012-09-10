#include <assert.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "net.h"

const size_t NET_CHUNKSIZE = 512;
const size_t NET_SERVICELEN = 64;

typedef struct addrinfo ADDRINFO;

ERROR_CODE net_connect(int* sock, const char* host, const long port) {
	ADDRINFO *addr, *p;
	char service[NET_SERVICELEN];
	int res = 0;
	
	snprintf(service, NET_SERVICELEN, "%ld", port);
	res = getaddrinfo(host, service, NULL, &addr);
	if (res != 0) {
		return ERR_NET_ADDRFAIL;
	}

	for(p = addr; p != NULL; p = p->ai_next) {
		if ((*sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
			continue;

		if (connect(*sock, p->ai_addr, p->ai_addrlen) == -1)
			continue;

		break;
	}

	freeaddrinfo(addr);

	if (!p) {
		*sock = 0;
		return ERR_NET_CONNFAIL;
	}

	return ERR_NONE;
}

ERROR_CODE net_send(int sock, const char* msg) {
	size_t nsent = 0;
	size_t size = strlen(msg);

	assert(sock != 0);

	nsent = send(sock, msg, size, 0);

	if(nsent < 0) {
		return ERR_NET_SENDFAIL;
	}

	return ERR_NONE;
}

ERROR_CODE net_recv_chunk(int sock, char* data, size_t* len) {
	assert(sock != 0);

	*len = recv(sock, data, NET_CHUNKSIZE, 0);

	if(*len == 0)
		return ERR_NET_CONNCLOSED;
	
	if(*len < 0)
		return ERR_NET_RECVFAIL;

	return ERR_NONE;
}

ERROR_CODE net_get(int sock, NET_HANDLER handle, NET_HANDLER_OBJ obj) {
	char buf[NET_CHUNKSIZE];
	size_t size = 0;
	ERROR_CODE err;

	assert(sock);

	while ((err = net_recv_chunk(sock, buf, &size)) == ERR_NONE) {
		if ((err = handle(obj, buf, size)) != ERR_NONE) {
			shutdown(sock, SHUT_RDWR);
			return err;
		}
	}
	
	/* TODO: Consider this postcondition */
	assert(err == ERR_NET_CONNCLOSED);
	
	return ERR_NONE;
}

ERROR_CODE net_free(int sock) {

	if(sock)
		shutdown(sock, SHUT_RDWR);
	
	return ERR_NONE;
}

