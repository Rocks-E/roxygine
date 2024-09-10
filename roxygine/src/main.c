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
#include "engine/entity/entity.h"

#define DEFAULT_FPS 60

typedef enum collision_layer_d {
	COLLISION_LAYER_NULL =		0b00000000,
	COLLISION_LAYER_PLAYER = 	0b00000001,
	COLLISION_LAYER_ENEMY = 	0b00000010,
	COLLISION_LAYER_TERRAIN = 	0b00000100
} collision_layer_e;

static char running = 1;

vec4 player_color = {0, 1, 1, 1};
u8 player_grounded = 0;

static void input_handle(body_t *body_player) {
	
	if(global.input.start)
		running = 0;
	
	f32 vx = 0;
	f32 vy = body_player->velocity[1];
	
	if(global.input.right)
		vx += 1000;
	if(global.input.left)
		vx -= 1000;
	
	if(global.input.up && player_grounded) {
		
		player_grounded = 0;
		
		vy = 4000;
		
	}
	
	body_player->velocity[0] = vx;
	body_player->velocity[1] = vy;
	
}

static void player_on_hit(body_t *self, body_t *other, hit_t hit) {
	
	if(other->collision_layer & COLLISION_LAYER_ENEMY) {
		
		player_color[0] = 1;
		player_color[2] = 0;
		
	}
	
	
}
static void player_on_hit_static(body_t *self, static_body_t *other, hit_t hit) {
	
	if(hit.normal[1] > 0)
		player_grounded = 1;
	
}

static void enemy_on_hit_static(body_t *self, static_body_t *other, hit_t hit) {
	
	if(hit.normal[0])
		self->velocity[0] = hit.normal[0] * 700;
	
}

static void game_init(void);
static void game_loop(void);
static void handle_sdl_events(void);

int32_t main(int32_t argc, char **argv) {
	
	
	game_init();
	game_loop();
	
	return 0;
	
}

static void game_init(void) {
	
	time_init(DEFAULT_FPS);
	config_init();
	render_init();
	physics_init();
	entity_init();
	
	SDL_ShowCursor(0);
	
}
static void game_loop(void) {
	
	u8 enemy_mask = COLLISION_LAYER_PLAYER | COLLISION_LAYER_TERRAIN;
	u8 player_mask = COLLISION_LAYER_ENEMY | COLLISION_LAYER_TERRAIN;
	
	size_t player_id = entity_create((vec2){100, 800}, (vec2){100, 100}, (vec2){0, 0}, COLLISION_LAYER_PLAYER, player_mask, player_on_hit, player_on_hit_static, 1);
	
	f32 width = global.render.width;
	f32 height = global.render.height;
	
	u32 static_body_a_id = physics_static_body_create((vec2){width * 0.5 - 25, height - 25}, (vec2){width - 50, 50}, COLLISION_LAYER_TERRAIN);
	u32 static_body_b_id = physics_static_body_create((vec2){width - 25, height * 0.5 + 25}, (vec2){50, height - 50}, COLLISION_LAYER_TERRAIN);
	u32 static_body_c_id = physics_static_body_create((vec2){width * 0.5 + 25, 25}, (vec2){width - 50, 50}, COLLISION_LAYER_TERRAIN);
	u32 static_body_d_id = physics_static_body_create((vec2){25, height * 0.5 - 25}, (vec2){50, height - 50}, COLLISION_LAYER_TERRAIN);
	u32 static_body_e_id = physics_static_body_create((vec2){width * 0.5, height * 0.5}, (vec2){150, 150}, COLLISION_LAYER_TERRAIN);
	
	size_t entity_a_id = entity_create((vec2){600, 600}, (vec2){50, 50}, (vec2){700, 0}, COLLISION_LAYER_ENEMY, enemy_mask, NULL, enemy_on_hit_static, 1);
	size_t entity_b_id = entity_create((vec2){800, 800}, (vec2){50, 50}, (vec2){700, 0}, COLLISION_LAYER_NULL, enemy_mask, NULL, enemy_on_hit_static, 1);
	
	while(running) {
		
		time_update();
		
		handle_sdl_events();
		
		entity_t *player = entity_get(player_id);
		body_t *body_player = physics_body_get(player->body_id);
		
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
		
		render_aabb((f32 *)body_player, player_color);
		
		render_aabb((f32 *)physics_body_get(entity_get(entity_a_id)->body_id), WHITE);
		render_aabb((f32 *)physics_body_get(entity_get(entity_b_id)->body_id), WHITE);
		
		render_end();

		player_color[0] = 0;
		player_color[2] = 1;
		
		time_update_late();
		
	}
	
}

static void handle_sdl_events(void) {
	
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
	
}