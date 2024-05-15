#ifndef RG_GLOBAL_H
#define RG_GLOBAL_H

#include "render/render.h"
#include "config/config.h"
#include "input/input.h"
#include "time/time.h"

typedef struct global_d {
	render_state_t render;
	config_state_t config;
	input_state_t input;
	time_state_t time;
} global_t;

extern global_t global;

#endif