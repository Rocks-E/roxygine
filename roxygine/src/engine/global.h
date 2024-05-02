#ifndef RG_GLOBAL_H
#define RG_GLOBAL_H

#include "render/render.h"

typedef struct global_d {
	render_state_t render;
} global_t;

extern global_t global;

#endif