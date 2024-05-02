#ifndef RG_IO_H
#define RG_IO_H

#include <stdlib.h>

#include "../types.h"

typedef struct file_d {
	
	char *data;
	size_t length;
	char is_valid;
	
} file_t;

file_t io_file_read(const char *file_path);
s32 io_file_write(void *buffer, size_t length, const char *file_path);

#endif