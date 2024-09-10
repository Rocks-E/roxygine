#ifndef RG_PHYSICS_H
#define RG_PHYSICS_H

#include <linmath.h>
#include "../types.h"

#define SIGN(x) ((x > 0) - (x < 0))

typedef struct body_d body_t;
typedef struct static_body_d static_body_t;
typedef struct hit_d hit_t;

typedef void (*on_hit_f)(body_t *self, body_t *other, hit_t hit);
typedef void (*on_hit_static_f)(body_t *self, static_body_t *other, hit_t hit);

typedef struct aabb_d {
	vec2 position;
	vec2 half_size;
} aabb_t;

struct body_d {
	aabb_t aabb;
	vec2 velocity;
	vec2 acceleration;
	on_hit_f on_hit;
	on_hit_static_f on_hit_static;
	u8 collision_layer;
	u8 collision_mask;
};

struct static_body_d {
	aabb_t aabb;
	u8 collision_layer;
};

struct hit_d {
	u8 is_hit;
	size_t other_id;
	f32 time;
	vec2 position;
	vec2 normal;
};

void physics_init(void);
void physics_update(void);
size_t physics_body_create(vec2 position, vec2 size, vec2 velocity, u8 collision_layer, u8 collision_mask, on_hit_f on_hit, on_hit_static_f on_hit_static);
body_t *physics_body_get(size_t index);
size_t physics_static_body_create(vec2 position, vec2 size, u8 collision_layer);
static_body_t *physics_static_body_get(size_t index);
void aabb_min_max(vec2 min, vec2 max, aabb_t aabb);
u8 physics_point_intersect_aabb(vec2 point, aabb_t aabb);
aabb_t aabb_minkowski_difference(aabb_t a, aabb_t b);
u8 physics_aabb_intersect_aabb(aabb_t a, aabb_t b);
void aabb_penetration_vector(vec2 r, aabb_t aabb);
hit_t ray_intersect_aabb(vec2 position, vec2 magnitude, aabb_t aabb);

#endif