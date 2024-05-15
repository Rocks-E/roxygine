#ifndef RG_CONFIG_H
#define RG_CONFIG_H

#include "../input/input.h"
#include "../types.h"

#define KEYBIND_COUNT 12

typedef struct config_state_d {
	u8 keybinds[KEYBIND_COUNT];
} config_state_t;

void config_init(void);
void config_key_bind(input_key key, const char *key_name);

#endif