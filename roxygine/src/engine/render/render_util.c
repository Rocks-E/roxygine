#include <glad/glad.h>
#include <stdio.h>

#include "../types.h"
#include "../util.h"
#include "../io/io.h"
#include "render_internal.h"

#define INFO_LOG_SIZE 512

u32 render_shader_create(const char *path_vert, const char *path_frag) {
	
	s32 success;
	char log[INFO_LOG_SIZE];
	
	// Read in the vertex shader file, ensure it is valid
	file_t file_vertex = io_file_read(path_vert);
	if(!file_vertex.is_valid) {
		ERROR_EXIT(1, "Error reading vertex shader: %s\n", path_vert);
	}
	
	// Create and compile the GL shader, ensure it was successfully compiled
	u32 shader_vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(shader_vertex, 1, (const char *const *)&file_vertex, NULL);
	glCompileShader(shader_vertex);
	glGetShaderiv(shader_vertex, GL_COMPILE_STATUS, &success);
	
	if(!success) {
		
		glGetShaderInfoLog(shader_vertex, INFO_LOG_SIZE, NULL, log);
		
		ERROR_EXIT(2, "Error compiling vertex shader: %s\n", log);
		
	}
	
	file_t file_fragment = io_file_read(path_frag);
	if(!file_fragment.is_valid) {
		ERROR_EXIT(1, "Error reading fragment shader: %s\n", path_frag);
	}
	
	u32 shader_fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(shader_fragment, 1, (const char *const *)&file_fragment, NULL);
	glCompileShader(shader_fragment);
	glGetShaderiv(shader_fragment, GL_COMPILE_STATUS, &success);
	
	if(!success) {
		
		glGetShaderInfoLog(shader_fragment, INFO_LOG_SIZE, NULL, log);
		
		ERROR_EXIT(2, "Error compiling vertex shader: %s\n", log);
		
	}
	
	u32 shader = glCreateProgram();
	glAttachShader(shader, shader_vertex);
	glAttachShader(shader, shader_fragment);
	glLinkProgram(shader);
	glGetProgramiv(shader, GL_LINK_STATUS, &success);
	
	if(!success) {
		
		glGetProgramInfoLog(shader, INFO_LOG_SIZE, NULL, log);
		
		ERROR_EXIT(3, "Error linking program: %s\n", log);
		
	}
	
	FILE_FREE(file_vertex);
	FILE_FREE(file_fragment);
	
	return shader;
	
}