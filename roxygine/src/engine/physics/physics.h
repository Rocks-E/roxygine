#ifndef RG_PHYSICS_H
#define RG_PHYSICS_H

#include "../types.h"

typedef struct aabb_d {
	vec2 position;
	vec2 half_size;
} aabb_t;

typedef struct body_d {
	aabb_t aabb;
	vec2 velocity;
	vec2 acceleration;
} body_t;

void physics_init(void);
void physics_update(void);
size_t physics_body_create(vec2 position, vec2 size);
body_t *physics_body_get(size_t index);

#endif