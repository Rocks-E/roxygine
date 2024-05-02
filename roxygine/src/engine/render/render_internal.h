#ifndef RG_RENDER_INTERNAL_H
#define RG_RENDER_INTERNAL_H

#include <SDL2/SDL.h>

#include "../types.h"
#include "render.h"

typedef struct render_state_internal_d {
	// Vertex Array Object
	u32 vao_quad;
	// Vertex Buffer Object
	u32 vbo_quad;
	// Element Buffer Object
	u32 ebo_quad;
} render_state_internal_t;

SDL_Window *render_init_window(u32 width, u32 height);
void render_init_quad(u32 *vao_quad, u32 *vbo_quad, u32 *ebo_quad);

#endif