#ifndef RG_PHYSICS_INTERNAL_H
#define RG_PHYSICS_INTERNAL_H

#include "../data_structures/array_list.h"
#include "../types.h"

typedef struct physics_state_internal_d {
	f32 gravity;
	f32 terminal_velocity;
	array_list_t *body_list;
	array_list_t *static_body_list;
} physics_state_internal_t;

#endif