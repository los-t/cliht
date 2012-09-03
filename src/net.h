#ifndef CLIHT_NET_H_
#define CLIHT_NET_H_

#include <stdlib.h>    /* for size_t */

#include "error.h"

extern const size_t NET_CHUNKSIZE;

ERROR_CODE net_connect(int* sock, const char* host, const long port);
ERROR_CODE net_send(int sock, const char* msg);
ERROR_CODE net_get_until(int sock, char** buf, const char* terminator);
ERROR_CODE net_get_count(int sock, char** buf, const size_t count);
ERROR_CODE net_free(int sock);
ERROR_CODE net_recv_byte(int sock, char* byte);

#endif // CLIHT_NET_H_
