#include "input.h"
#include "../global.h"
#include "../types.h"

static void update_key_state(u8 current_state, key_state *state) {
	
	if(current_state)
		*state = (*state > 0) ? KEY_STATE_HELD : KEY_STATE_PRESSED;
	else
		*state = KEY_STATE_UNPRESSED;
	
}

void input_update(void) {
	
	const u8 *keyboard_state = SDL_GetKeyboardState(NULL);
	
	update_key_state(keyboard_state[global.config.keybinds[INPUT_KEY_LEFT]], &global.input.left);
	update_key_state(keyboard_state[global.config.keybinds[INPUT_KEY_RIGHT]], &global.input.right);
	update_key_state(keyboard_state[global.config.keybinds[INPUT_KEY_UP]], &global.input.up);
	update_key_state(keyboard_state[global.config.keybinds[INPUT_KEY_DOWN]], &global.input.down);
	update_key_state(keyboard_state[global.config.keybinds[INPUT_KEY_START]], &global.input.start);
	update_key_state(keyboard_state[global.config.keybinds[INPUT_KEY_SELECT]], &global.input.select);
	update_key_state(keyboard_state[global.config.keybinds[INPUT_KEY_A]], &global.input.a);
	update_key_state(keyboard_state[global.config.keybinds[INPUT_KEY_B]], &global.input.b);
	update_key_state(keyboard_state[global.config.keybinds[INPUT_KEY_X]], &global.input.x);
	update_key_state(keyboard_state[global.config.keybinds[INPUT_KEY_Y]], &global.input.y);
	update_key_state(keyboard_state[global.config.keybinds[INPUT_KEY_LEFT_SHOULDER]], &global.input.left_shoulder);
	update_key_state(keyboard_state[global.config.keybinds[INPUT_KEY_RIGHT_SHOULDER]], &global.input.right_shoulder);
	
}