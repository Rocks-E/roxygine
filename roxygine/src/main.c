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

static char running = 1;
static vec2 pos;

static void input_handle(void) {
	if(global.input.left == KEY_STATE_PRESSED || global.input.left == KEY_STATE_HELD)
		pos[0] -= 500 * global.time.delta;
	if(global.input.right == KEY_STATE_PRESSED || global.input.right == KEY_STATE_HELD)
		pos[0] += 500 * global.time.delta;
	if(global.input.up == KEY_STATE_PRESSED || global.input.up == KEY_STATE_HELD)
		pos[1] += 500 * global.time.delta;
	if(global.input.down == KEY_STATE_PRESSED || global.input.down == KEY_STATE_HELD)
		pos[1] -= 500 * global.time.delta;
}

int32_t main(int32_t argc, char **argv) {
	
	time_init(60);
	config_init();
	render_init();
	
	pos[0] = global.render.width * 0.5;
	pos[1] = global.render.height * 0.5;
	
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
		
		render_begin();
	
		render_quad(
					pos,
					(vec2){50, 50},
					(vec4){1, 0, 1, 1}
					);
					
		render_end();
		time_update_late();
		
	}
	
	return 0;
	
}