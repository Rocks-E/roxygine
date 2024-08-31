#include "../global.h"
#include "../util.h"
#include "physics.h"
#include "physics_internal.h"

static physics_state_internal_t phys_state;

void physics_init(void) {
	phys_state.body_list = array_list_new(sizeof(body_t), 0);
}

void physics_update(void) {
	
	body_t *body_cursor;
	
	for(size_t c = 0; c < phys_state.body_list->length; c++) {
		
		body_cursor = (body_t *)array_list_get(phys_state.body_list, c);
		
		//fprintf(stdout, "Body index: %zu\n\tBody pos: (%f, %f)\n\tBody vel: (%f, %f)\n", c, body_cursor->aabb.position[0], body_cursor->aabb.position[1], body_cursor->velocity[0], body_cursor->velocity[1]);
		
		body_cursor->velocity[0] += body_cursor->acceleration[0] * global.time.delta;
		body_cursor->velocity[1] += body_cursor->acceleration[1] * global.time.delta;
		body_cursor->aabb.position[0] += body_cursor->velocity[0] * global.time.delta;
		body_cursor->aabb.position[1] += body_cursor->velocity[1] * global.time.delta;
		
		//fprintf(stdout, "Body index: %zu\n\tBody pos: (%f, %f)\n\tBody vel: (%f, %f)\n", c, body_cursor->aabb.position[0], body_cursor->aabb.position[1], body_cursor->velocity[0], body_cursor->velocity[1]);
		
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