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
	u32 shader_default;
	u32 texture_color;
	mat4x4 projection;
} render_state_internal_t;

SDL_Window *render_init_window(u32 width, u32 height);
void render_init_quad(u32 *vao_quad, u32 *vbo_quad, u32 *ebo_quad);
void render_init_color_texture(u32 *texture);
void render_init_shaders(render_state_internal_t *state);
u32 render_shader_create(const char *path_vert, const char *path_frag);

#endif