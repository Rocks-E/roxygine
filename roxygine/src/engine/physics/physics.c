#include "../global.h"
#include "../util.h"
#include "physics.h"
#include "physics_internal.h"

static physics_state_internal_t phys_state;
static u32 iterations = 4;
static f32 tick_rate;

void physics_init(void) {
	
	phys_state.body_list = array_list_new(sizeof(body_t), 0);
	phys_state.static_body_list = array_list_new(sizeof(static_body_t), 0);
	
	phys_state.gravity = -200;
	phys_state.terminal_velocity = -10000;
	
	tick_rate = 1.f / iterations;
	
}

static hit_t sweep_static_bodies(aabb_t aabb, vec2 velocity) {
	
	hit_t result = {.time = 0xFFFF};
	
	for(u32 c = 0; c < phys_state.static_body_list->length; c++) {
		
		static_body_t *static_body_cursor = physics_static_body_get(c);
		
		aabb_t sum_aabb = static_body_cursor->aabb;
		vec2_add(sum_aabb.half_size, sum_aabb.half_size, aabb.half_size);
		
		hit_t hit = ray_intersect_aabb(aabb.position, velocity, sum_aabb);
		if(!hit.is_hit)
			continue;
		
		if(hit.time < result.time)
			result = hit;
		else if(hit.time == result.time)
			if((fabsf(velocity[0]) > fabsf(velocity[1])) && hit.normal[0])
				result = hit;
			else if((fabsf(velocity[0]) < fabsf(velocity[1])) && hit.normal[1])
				result = hit;
		
	}
	
	return result;
	
}

static void sweep_response(body_t *body, vec2 velocity) {
	
	hit_t hit = sweep_static_bodies(body->aabb, velocity);
	
	if(hit.is_hit) {
		
		body->aabb.position[0] = hit.position[0];
		body->aabb.position[1] = hit.position[1];
		
		if(hit.normal[0]) {
			
			body->velocity[0] = 0;
			body->aabb.position[1] += velocity[1];
			
		}
		else if(hit.normal[1]) {
			
			body->aabb.position[0] += velocity[0];
			body->velocity[1] = 0;
			
		}
		
	}
	else 
		vec2_add(body->aabb.position, body->aabb.position, velocity);
	
}

static void stationary_response(body_t *body) {
	
	for(u32 c = 0; c < phys_state.static_body_list->length; c++) {
		
		static_body_t *static_body_cursor = physics_static_body_get(c);
		
		aabb_t aabb = aabb_minkowski_difference(static_body_cursor->aabb, body->aabb);
		
		if(physics_point_intersect_aabb((vec2){0, 0}, aabb)) {
			
			vec2 penetration_vector;
			aabb_penetration_vector(penetration_vector, aabb);
			
			vec2_add(body->aabb.position, body->aabb.position, penetration_vector);
			
		}
		
	}
	
}

void physics_update(void) {
	
	body_t *body_cursor;
	
	for(size_t c = 0; c < phys_state.body_list->length; c++) {
		
		body_cursor = (body_t *)array_list_get(phys_state.body_list, c);
		
		body_cursor->velocity[1] += phys_state.gravity;
		if(phys_state.terminal_velocity > body_cursor->velocity[1])
			body_cursor->velocity[1] = phys_state.terminal_velocity;
		
		body_cursor->velocity[0] += body_cursor->acceleration[0];
		body_cursor->velocity[1] += body_cursor->acceleration[1];
		
		vec2 scaled_velocity;
		vec2_scale(scaled_velocity, body_cursor->velocity, global.time.delta * tick_rate);
		
		for(u32 i = 0; i < iterations; i++) {
			
			sweep_response(body_cursor, scaled_velocity);
			stationary_response(body_cursor);
			
		}
		
	}
	
}

size_t physics_body_create(vec2 position, vec2 size) {
	
	body_t new_body = {
		.aabb = {
			.position = {position[0], position[1]},
			.half_size = {size[0] * 0.5, size[1] * 0.5}
		},
		.velocity = {0, 0}
	};
	
	if(array_list_push(phys_state.body_list, &new_body) == (size_t)(-1))
		ERROR_EXIT(-1, "Could not append new body to physics list\n");
	
	return phys_state.body_list->length - 1;
	
}

body_t *physics_body_get(size_t index) {
	return array_list_get(phys_state.body_list, index);
}

void aabb_min_max(vec2 min, vec2 max, aabb_t aabb) {
	
	vec2_sub(min, aabb.position, aabb.half_size);
	vec2_add(max, aabb.position, aabb.half_size);
	
}

size_t physics_static_body_create(vec2 position, vec2 size) {
	
	static_body_t new_static_body = {
		.aabb = {
			.position = {position[0], position[1]},
			.half_size = {size[0] * 0.5, size[1] * 0.5}
		}
	};
	
	if(array_list_push(phys_state.static_body_list, &new_static_body) == (size_t)(-1))
		ERROR_EXIT(-1, "Could not append new static body to physics list\n");
	
	return phys_state.static_body_list->length - 1;
	
}

static_body_t *physics_static_body_get(size_t index) {
	return array_list_get(phys_state.static_body_list, index);
}

u8 physics_point_intersect_aabb(vec2 point, aabb_t aabb) {
	
	vec2 min, max;
	aabb_min_max(min, max, aabb);
	
	return (point[0] >= min[0] && point[0] <= max[0]) && (point[1] >= min[1] && point[1] <= max[1]);
	
}

aabb_t aabb_minkowski_difference(aabb_t a, aabb_t b) {
	
	aabb_t result;
	
	vec2_sub(result.position, a.position, b.position);
	vec2_add(result.half_size, a.half_size, b.half_size);
	
	return result;
	
}

u8 physics_aabb_intersect_aabb(aabb_t a, aabb_t b) {
	
	aabb_t minkowski_difference = aabb_minkowski_difference(a, b);
	
	return physics_point_intersect_aabb((vec2){0, 0}, minkowski_difference);
	
}

void aabb_penetration_vector(vec2 r, aabb_t aabb) {
	
	vec2 min, max;
	aabb_min_max(min, max, aabb);
	
	f32 minimum_distance = fabsf(min[0]);
	r[0] = min[0];
	r[1] = 0;

	f32 test_distance = fabsf(max[0]);
	
	if(test_distance < minimum_distance) {
		
		minimum_distance = test_distance;
		r[0] = max[0];
		
	}
	
	test_distance = fabsf(min[1]);
	
	if(test_distance < minimum_distance) {
		
		minimum_distance = test_distance;
		r[0] = 0;
		r[1] = min[1];
		
	}
	
	test_distance = fabsf(max[1]);
	
	if(test_distance < minimum_distance) {
		
		minimum_distance = test_distance;
		r[0] = 0;
		r[1] = max[1];
		
	}
	
}

hit_t ray_intersect_aabb(vec2 position, vec2 magnitude, aabb_t aabb) {
	
	hit_t hit = {0};
	
	vec2 min, max;
	aabb_min_max(min, max, aabb);
	
	f32 last_entry = -INFINITY;
	f32 first_exit = INFINITY;
	
	for(u8 c = 0; c < 2; c++) {
		
		if(magnitude[c]) {
			
			f32 t1 = (min[c] - position[c]) / magnitude[c];
			f32 t2 = (max[c] - position[c]) / magnitude[c];
			
			last_entry = fmaxf(last_entry, fminf(t1, t2));
			first_exit = fminf(first_exit, fmaxf(t1, t2));
			
			
		}
		else if((position[c] <= min[c]) || (position[c] >= max[c])) {
			return hit;
		}
		
	}
	
	if((first_exit > last_entry) && (first_exit > 0) && (last_entry < 1)) {
		
		hit.position[0] = position[0] + magnitude[0] * last_entry;
		hit.position[1] = position[1] + magnitude[1] * last_entry;
		
		hit.is_hit = 1;
		hit.time = last_entry;
		
		f32 dx = hit.position[0] - aabb.position[0];
		f32 dy = hit.position[1] - aabb.position[1];
		f32 px = aabb.half_size[0] - fabsf(dx);
		f32 py = aabb.half_size[1] - fabsf(dy);
		
		if(px < py)
			hit.normal[0] = SIGN(dx);
		else
			hit.normal[1] = SIGN(dy);
		
	}
	
	return hit;
	
}