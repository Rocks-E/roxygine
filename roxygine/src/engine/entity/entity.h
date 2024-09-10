#ifndef RG_ENTITY_H
#define RG_ENTITY_H

#include "../physics/physics.h"
#include "../data_structures/array_list.h"

typedef struct entity_d {
	size_t body_id;
} entity_t;

void entity_init(void);
size_t entity_create(vec2 position, vec2 size, vec2 velocity, u8 collision_layer, u8 collision_mask, on_hit_f on_hit, on_hit_static_f on_hit_static, u8 is_active);
entity_t *entity_get(size_t id);
size_t entity_count(void);

#endif