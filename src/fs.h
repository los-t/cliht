#ifndef CLIHT_FS_H_
#define CLIHT_FS_H_

#include <stdio.h>

#include "error.h"

typedef struct FileStruct {
	FILE *file;
	char *path;
	unsigned int  is_open;
} File;

ERROR_CODE fs_create(File** /*file*/, const char* /*path*/);
ERROR_CODE fs_free(File** /*file*/);

ERROR_CODE fs_open(File* /*file*/);
ERROR_CODE fs_close(File* /*file*/);

ERROR_CODE fs_write(File* /*file*/, const char* /*data*/, const size_t /*size*/);

#endif // CLIHT_FS_H_
