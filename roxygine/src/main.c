#include <stdio.h>
#include <stdint.h>
#include <glad/glad.h>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

#include "engine/global.h"
#include "engine/util.h"
#include "engine/config/config.h"
#include "engine/input/input.h"
#include "engine/time/time.h"
#include "engine/render/render.h"
#include "engine/render/render_internal.h"
#include "engine/physics/physics.h"

static char running = 1;
static vec2 pos;

static void input_handle(void) {
	
	s32 x, y;
	SDL_GetMouseState(&x, &y);
	
	pos[0] = (f32)x;
	pos[1] = global.render.height - y;
	
}

int32_t main(int32_t argc, char **argv) {
	
	time_init(60);
	config_init();
	render_init();
	physics_init();
	
	SDL_ShowCursor(0);
	
	pos[0] = global.render.width * 0.5;
	pos[1] = global.render.height * 0.5;
	
	aabb_t test_aabb = {
		.position = {global.render.width * 0.5, global.render.height * 0.5},
		.half_size = {50, 50}
	};
	
	while(running) {
		
		time_update();
		
		SDL_Event event;
		
		while(SDL_PollEvent(&event)) {
			
			switch(event.type) {
				
				case SDL_QUIT:
					running = 0;
					break;
					
				default:
					break;
				
			}
			
		}
		
		input_update();
		input_handle();
		physics_update();
		
		render_begin();
	
		/*
		render_quad(
					pos,
					(vec2){50, 50},
					(vec4){1, 0, 1, 1}
					);
		*/
		
		render_aabb((f32 *)&test_aabb, (vec4){1, 1, 1, 0.5});
		render_quad(pos, (vec2){5, 5}, (physics_point_intersect_aabb(pos, test_aabb) ? RED : WHITE));
		
		render_end();		
		time_update_late();
		
	}
	
	return 0;
	
}