#ifndef RG_RENDER_H
#define RG_RENDER_H

#include <SDL2/SDL.h>
#include <linmath.h>

#include "../types.h"

typedef struct render_state_d {
	SDL_Window *window;
	f32 width;
	f32 height;
} render_state_t;

void render_init(void);
void render_begin(void);
void render_end(void);
void render_quad(vec2 position, vec2 size, vec4 color);

#endif