#ifndef CLIHT_NET_H_
#define CLIHT_NET_H_

#include <stdlib.h>    /* for size_t */

#include "error.h"

extern const size_t NET_CHUNKSIZE;

typedef void* NET_HANDLER_OBJ;
typedef ERROR_CODE (*NET_HANDLER)(NET_HANDLER_OBJ, const char*, const size_t);

ERROR_CODE net_connect(int* /*sock*/, const char* /*host*/, const long /*port*/);
ERROR_CODE net_send(int /*sock*/, const char* /*msg*/);
ERROR_CODE net_get(int /*sock*/, NET_HANDLER /*handle*/, NET_HANDLER_OBJ /*hobj*/);
ERROR_CODE net_free(int /*sock*/);

#endif // CLIHT_NET_H_
