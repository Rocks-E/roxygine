#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "../types.h"
#include "../util.h"
#include "io.h"

// 20 MiB chunk size, could be increased
#define IO_READ_CHUNK_SIZE 2097152

#define IO_READ_ERROR_GENERAL "Error reading file: %s. errno: %d\n"
#define IO_READ_ERROR_MEMORY "Not enough free memory to read file: %s\n"

file_t io_file_read(const char *file_path) {
	
	file_t file = {.is_valid = false};
	
	FILE *file_ptr = fopen(file_path, "rb");
	if(ferror(file_ptr)) {
		ERROR_RETURN(file, IO_READ_ERROR_GENERAL, file_path, errno);
	}
	
	char *data = NULL, *tmp;
	size_t used = 0, size = 0, n;
	
	while(true) {
		
		if(used + IO_READ_CHUNK_SIZE + 1 > size) {
			
			size = used + IO_READ_CHUNK_SIZE + 1;
			
			if(size <= used) {
				
				if(data)
					free(data);
				
				ERROR_RETURN(file, "Input file too large %s\n", file_path);
				
			}
			
			tmp = (char *)realloc(data, size);
			if(!tmp) {
				
				if(data)
					free(data);
				
				ERROR_RETURN(file, IO_READ_ERROR_MEMORY, file_path);
				
			}
			
			data = tmp;
			
		}
		
		n = fread(data + used, 1, IO_READ_CHUNK_SIZE, file_ptr);
		if(!n)
			break;
		
		used += n;
		
	}
	
	if(ferror(file_ptr)) {
		
		if(data)
			free(data);
		
		ERROR_RETURN(file, IO_READ_ERROR_GENERAL, file_path, errno);
		
	}
	
	tmp = (char *)realloc(data, used + 1);
	if(!tmp) {
		
		if(data)
			free(data);
		
		ERROR_RETURN(file, IO_READ_ERROR_MEMORY, file_path);
		
	}
	
	data = tmp;
	data[used] = 0;
	file.data = data;
	file.length = used;
	file.is_valid = 1;
	
	return file;
	
}

s32 io_file_write(void *buffer, size_t length, const char *file_path) {
	
}