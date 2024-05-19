#include "config.h"
#include "../global.h"
#include "../io/io.h"
#include "../util.h"

static const char *CONFIG_DEFAULT = "[controls]\n"
									"left = Left\n"
									"right = Right\n"
									"up = Up\n"
									"down = Down\n"
									"start = Return\n"
									"select = Tab\n"
									"a = Left Shift\n"
									"b = X\n"
									"x = Z\n"
									"y = C\n"
									"left_shoulder = A\n"
									"right_shoulder = S\n"
									"\n";

static char tmp_buffer[25] = {0};

static char *config_get_value(const char *config_buffer, const char *name) {
	
	char *line = strstr(config_buffer, name);
	
	if(!line)
		ERROR_EXIT(1, "Could not find config value: %s. Try deleting config.ini and restarting.\n", name);
	
	char *line_end = line + strlen(line);
	char *cursor = line;
	char *tmp_ptr = tmp_buffer;
	
	// Move to = and consume
	while(*cursor != '=' && cursor != line_end) cursor++;
	cursor++;
	// Consume any spaces
	while(*cursor == ' ') cursor++;
	
	while(*cursor != '\n' && *cursor != 0 && cursor != line_end)
		*(tmp_ptr++) = *(cursor++);
	
	*(tmp_ptr) = 0;
	
	return tmp_buffer;
	
}

static void load_controls(const char *config_buffer) {
	
	config_key_bind(INPUT_KEY_LEFT, 			config_get_value(config_buffer, "left"));
	config_key_bind(INPUT_KEY_RIGHT, 			config_get_value(config_buffer, "right"));
	config_key_bind(INPUT_KEY_UP, 				config_get_value(config_buffer, "up"));
	config_key_bind(INPUT_KEY_DOWN, 			config_get_value(config_buffer, "down"));
	config_key_bind(INPUT_KEY_START, 			config_get_value(config_buffer, "start"));
	config_key_bind(INPUT_KEY_SELECT, 			config_get_value(config_buffer, "select"));
	config_key_bind(INPUT_KEY_A, 				config_get_value(config_buffer, "a"));
	config_key_bind(INPUT_KEY_B, 				config_get_value(config_buffer, "b"));
	config_key_bind(INPUT_KEY_X, 				config_get_value(config_buffer, "x"));
	config_key_bind(INPUT_KEY_Y, 				config_get_value(config_buffer, "y"));
	config_key_bind(INPUT_KEY_LEFT_SHOULDER, 	config_get_value(config_buffer, "left_shoulder"));
	config_key_bind(INPUT_KEY_RIGHT_SHOULDER, 	config_get_value(config_buffer, "right_shoulder"));
	
}

static s32 config_load(void) {
	
	file_t file_config = io_file_read("./config.ini");
	
	if(!file_config.is_valid)
		return 1;
	
	load_controls(file_config.data);
	
	FILE_FREE(file_config);
	
	return 0;
	
}

void config_init(void) {

	if(!config_load())
		return;
	
	io_file_write((void *)CONFIG_DEFAULT, strlen(CONFIG_DEFAULT), "./config.ini");
	
	if(config_load())
		ERROR_EXIT(1, "Could not create or load config file\n");
	
}

void config_key_bind(input_key key, const char *key_name) {
	
	SDL_Scancode scan_code = SDL_GetScancodeFromName(key_name);
	if(scan_code == SDL_SCANCODE_UNKNOWN)
		ERROR_EXIT(1, "Invalid scan code when binding key: %s\n", key_name);
	
	global.config.keybinds[key] = scan_code;
	
}