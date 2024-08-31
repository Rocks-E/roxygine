#ifndef RG_RENDER_H
#define RG_RENDER_H

#include <SDL2/SDL.h>
#include <linmath.h>

#include "../types.h"

#define DEFAULT_LINE_WIDTH 3

typedef struct render_state_d {
	SDL_Window *window;
	f32 width;
	f32 height;
} render_state_t;

void render_init(void);
void render_begin(void);
void render_end(void);
void render_quad(vec2 position, vec2 size, vec4 color);
void render_line_segment(vec2 start_position, vec2 end_position, vec4 color);
void render_quad_line(vec2 position, vec2 size, vec4 color);
void render_aabb(f32 *aabb, vec4 color);

#endif