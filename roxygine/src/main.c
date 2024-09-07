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

static void input_handle(body_t *body_player) {
	
	if(global.input.start)
		running = 0;
	
	f32 vx = 0;
	f32 vy = body_player->velocity[1];
	
	if(global.input.right)
		vx += 1000;
	if(global.input.left)
		vx -= 1000;
	
	if(global.input.up)
		vy = 4000;
	
	body_player->velocity[0] = vx;
	body_player->velocity[1] = vy;
	
}

int32_t main(int32_t argc, char **argv) {
	
	time_init(60);
	config_init();
	render_init();
	physics_init();
	
	SDL_ShowCursor(0);
	
	u32 body_id = physics_body_create((vec2){100, 800}, (vec2){50, 50});
	
	f32 width = global.render.width;
	f32 height = global.render.height;
	
	u32 static_body_a_id = physics_static_body_create((vec2){width * 0.5 - 25, height - 25}, (vec2){width - 50, 50});
	u32 static_body_b_id = physics_static_body_create((vec2){width - 25, height * 0.5 + 25}, (vec2){50, height - 50});
	u32 static_body_c_id = physics_static_body_create((vec2){width * 0.5 + 25, 25}, (vec2){width - 50, 50});
	u32 static_body_d_id = physics_static_body_create((vec2){25, height * 0.5 - 25}, (vec2){50, height - 50});
	u32 static_body_e_id = physics_static_body_create((vec2){width * 0.5, height * 0.5}, (vec2){150, 150});
	
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
		
		body_t *body_player = physics_body_get(body_id);
		
		static_body_t *static_body_a = physics_static_body_get(static_body_a_id);
		static_body_t *static_body_b = physics_static_body_get(static_body_b_id);
		static_body_t *static_body_c = physics_static_body_get(static_body_c_id);
		static_body_t *static_body_d = physics_static_body_get(static_body_d_id);
		static_body_t *static_body_e = physics_static_body_get(static_body_e_id);
		
		input_update();
		input_handle(body_player);
		physics_update();
		
		render_begin();
		
		render_aabb((f32 *)static_body_a, WHITE);
		render_aabb((f32 *)static_body_b, WHITE);
		render_aabb((f32 *)static_body_c, WHITE);
		render_aabb((f32 *)static_body_d, WHITE);
		render_aabb((f32 *)static_body_e, WHITE);
		
		render_aabb((f32 *)body_player, CYAN);
		
		render_end();		
		time_update_late();
		
	}
	
	return 0;
	
}