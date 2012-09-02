#include <stdlib.h>
#include <string.h>

#include "utl.h"

ERROR_CODE init_from_str(char** dest, const char* src, const size_t len) {
	*dest = (char*)malloc((len + 1)*sizeof(*src));
	memset(*dest, 0, (len + 1)*sizeof(*src));
	strncpy(*dest, src, len);
	
	return ERR_NONE;
}
