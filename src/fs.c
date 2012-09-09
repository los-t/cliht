#define _GNU_SOURCE
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "error.h"

#include "fs.h"

ERROR_CODE fs_create(File** file, const char* path) {
	assert(file != NULL);

	*file = (File*)malloc(sizeof(File));

	(*file)->is_open = 0;

	if (path) {
		(*file)->path = strdup(path);
	} else {
		(*file)->path = NULL;
	}

	return ERR_NONE;
}

ERROR_CODE fs_free(File** file) {
	assert(*file != NULL);

	fs_close(*file);

	free((*file)->path);
	free(*file);
	*file = NULL;

	return ERR_NONE;
}

ERROR_CODE fs_open(File* file) {
	assert(file != NULL);

	if (file->is_open)
		return ERR_NONE;

	assert(file->path != NULL);

	file->file = fopen(file->path, "a");
	if (!file->file) {
		return ERR_FILE_OPENFAIL;
	}
	file->is_open = 1;

	return ERR_NONE;
}

ERROR_CODE fs_close(File* file) {
	assert(file != NULL);

	if(!file->is_open)
		return ERR_NONE;

	fclose(file->file);

	return ERR_NONE;
}

ERROR_CODE fs_write(File* file, const char* data, const size_t size) {
	size_t nbytes = 0;

	assert (file != NULL);
	assert (data != NULL);

	fs_open(file);

	nbytes = fwrite(data, sizeof(*data), size, file->file);
	if (!nbytes)
		return ERR_FILE_WRITEFAIL;
	
	return ERR_NONE;
}
