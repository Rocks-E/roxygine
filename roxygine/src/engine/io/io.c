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
	
	file_t file = {.is_valid = 0};
	
	FILE *file_ptr = fopen(file_path, "rb");
	if(ferror(file_ptr)) {
		ERROR_RETURN(file, IO_READ_ERROR_GENERAL, file_path, errno);
	}
	
	char *data = NULL, *tmp;
	size_t used = 0, size = 0, n;
	
	// Loop forever, break out when we have finished reading the file
	while(1) {
		
		// If after reading the next chunk we will exceed the current data size, increase the buffer size
		if(used + IO_READ_CHUNK_SIZE + 1 > size) {
			
			// Increase the size to the current used amount plus an additional chunk
			size = used + IO_READ_CHUNK_SIZE + 1;
			
			// Error if out of memory to load this
			if(size <= used) {
				
				if(data)
					free(data);
				
				ERROR_RETURN(file, "Input file too large %s\n", file_path);
				
			}
			
			// Allocate more memory
			tmp = (char *)realloc(data, size);
			if(!tmp) {
				
				if(data)
					free(data);
				
				ERROR_RETURN(file, IO_READ_ERROR_MEMORY, file_path);
				
			}
			
			// Set data to the reallocated memory block
			data = tmp;
			
		}
		
		// Read a chunk from the file into the data stream
		n = fread(data + used, 1, IO_READ_CHUNK_SIZE, file_ptr);
		if(!n)
			break;
		
		// Increase our datastream used count
		used += n;
		
	}
	
	// Check for any file errors
	if(ferror(file_ptr)) {
		
		if(data)
			free(data);
		
		ERROR_RETURN(file, IO_READ_ERROR_GENERAL, file_path, errno);
		
	}
	
	fclose(file_ptr);
	
	// Allocate space to ensure we can null terminate the datastream
	tmp = (char *)realloc(data, used + 1);
	if(!tmp) {
		
		if(data)
			free(data);
		
		ERROR_RETURN(file, IO_READ_ERROR_MEMORY, file_path);
		
	}
	
	// Assign data back to the reallocated stream, null terminate
	data = tmp;
	data[used] = 0;
	
	// Assign the data attributes to the return file struct
	file.data = data;
	file.length = used;
	file.is_valid = 1;
	
	return file;
	
}

s32 io_file_write(void *buffer, size_t length, const char *file_path) {
	return 1;
}