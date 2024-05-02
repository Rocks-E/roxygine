#include <glad/glad.h>
#include <SDL2/SDL.h>

#include "../util.h"
#include "../global.h"

#include "render.h"
#include "render_internal.h"

SDL_Window *render_init_window(u32 width, u32 height) {
	
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		ERROR_EXIT(1, "Could not initialize SDL: %s\n", SDL_GetError());
	}
	
	SDL_Window *window = SDL_CreateWindow("Roxygine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
	
	if(!window) {
		ERROR_EXIT(2, "Failed to initialize window: %s\n", SDL_GetError());
	}
	
	SDL_GL_CreateContext(window);
	if(!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
		ERROR_EXIT(3, "Failed to load OpenGL: %s\n", SDL_GetError());
	}
	
	printf("OpenGL Loaded\n");
	printf("Vendor: %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Version: %s\n", glGetString(GL_VERSION));
	
	return window;
	
}

void render_init_quad(u32 *vao, u32 *vbo, u32 *ebo) {
	
	//	   x,	 y, z, u, v
	f32 vertices[] = {
		 0.5,  0.5, 0, 0, 0,
		 0.5, -0.5, 0, 0, 1,
		-0.5, -0.5, 0, 1, 1,
		-0.5,  0.5, 0, 1, 0
	};
	
	u32 indices[] = {
		0, 1, 3,
		1, 2, 3
	};
	
	glGenVertexArrays(1, vao);
	glGenBuffers(1, vbo);
	glGenBuffers(1, ebo);
	
	glBindVertexArray(*vao);
	
	glBindBuffer(GL_ARRAY_BUFFER, *vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
	// xyz
	// (0 -> attribute index, 3 -> how many elements (x, y, z = 3), GL_FLOAT -> type, GL_FALSE -> do not normalize float, 5 * sizeof(f32) -> each row has 5 elements, NULL -> no offset from beginning of the row)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(f32), NULL);
	glEnableVertexAttribArray(0);
	
	// uv
	// (1 -> attribute index, 2 -> how many elements (u, v = 2), GL_FLOAT -> same, GL_FALSE -> same, 5 * sizeof(f32) -> same, (void *)(3 * sizeof(f32)) -> 3 float offset from beginning of the row)
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(f32), (void *)(3 * sizeof(f32)));
	glEnableVertexAttribArray(1);
	
	glBindVertexArray(0);
	
}