#ifndef RG_INPUT_H
#define RG_INPUT_H

typedef enum input_key_e {
	INPUT_KEY_LEFT,
	INPUT_KEY_RIGHT,
	INPUT_KEY_UP,
	INPUT_KEY_DOWN,
	INPUT_KEY_START,
	INPUT_KEY_SELECT,
	INPUT_KEY_A,
	INPUT_KEY_B,
	INPUT_KEY_X,
	INPUT_KEY_Y,
	INPUT_KEY_LEFT_SHOULDER,
	INPUT_KEY_RIGHT_SHOULDER
} input_key;

typedef enum key_state_e {
	KEY_STATE_UNPRESSED,
	KEY_STATE_PRESSED,
	KEY_STATE_HELD
} key_state;

typedef struct input_state_d {
	key_state left;
	key_state right;
	key_state up;
	key_state down;
	key_state start;
	key_state select;
	key_state a;
	key_state b;
	key_state x;
	key_state y;
	key_state left_shoulder;
	key_state right_shoulder;
} input_state_t;

void input_update(void);

#endif