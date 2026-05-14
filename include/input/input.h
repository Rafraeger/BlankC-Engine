#ifndef INPUT_H
#define INPUT_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    KEY_SPACE = 32,
    KEY_APOSTROPHE = 39,
    KEY_COMMA = 44,
    KEY_MINUS = 45,
    KEY_PERIOD = 46,
    KEY_SLASH = 47,
    KEY_0 = 48,
    KEY_1 = 49,
    KEY_2 = 50,
    KEY_3 = 51,
    KEY_4 = 52,
    KEY_5 = 53,
    KEY_6 = 54,
    KEY_7 = 55,
    KEY_8 = 56,
    KEY_9 = 57,
    KEY_SEMICOLON = 59,
    KEY_EQUAL = 61,
    KEY_A = 65,
    KEY_B = 66,
    KEY_C = 67,
    KEY_D = 68,
    KEY_E = 69,
    KEY_F = 70,
    KEY_G = 71,
    KEY_H = 72,
    KEY_I = 73,
    KEY_J = 74,
    KEY_K = 75,
    KEY_L = 76,
    KEY_M = 77,
    KEY_N = 78,
    KEY_O = 79,
    KEY_P = 80,
    KEY_Q = 81,
    KEY_R = 82,
    KEY_S = 83,
    KEY_T = 84,
    KEY_U = 85,
    KEY_V = 86,
    KEY_W = 87,
    KEY_X = 88,
    KEY_Y = 89,
    KEY_Z = 90,
    KEY_LEFT_CONTROL = 341,
    KEY_LEFT_SHIFT = 340,
    KEY_LEFT_ALT = 342,
    KEY_LEFT_SUPER = 343,
    KEY_RIGHT_CONTROL = 344,
    KEY_RIGHT_SHIFT = 345,
    KEY_RIGHT_ALT = 346,
    KEY_RIGHT_SUPER = 347,
    KEY_ESCAPE = 256,
    KEY_ENTER = 257,
    KEY_TAB = 258,
    KEY_BACKSPACE = 259,
    KEY_INSERT = 260,
    KEY_DELETE = 261,
    KEY_RIGHT = 262,
    KEY_LEFT = 263,
    KEY_DOWN = 264,
    KEY_UP = 265,
    KEY_F1 = 290,
    KEY_F2 = 291,
    KEY_F3 = 292,
    KEY_F4 = 293,
    KEY_F5 = 294,
    KEY_F6 = 295,
    KEY_F7 = 296,
    KEY_F8 = 297,
    KEY_F9 = 298,
    KEY_F10 = 299,
    KEY_F11 = 300,
    KEY_F12 = 301
} Key;

typedef struct {
    int keys[512];
    int prev_keys[512];
    float mouse_x;
    float mouse_y;
    float dx;
    float dy;
    int mouse_left;
    int mouse_right;
    int prev_mouse_left;
    int prev_mouse_right;
} Input;

Input* input_create(void);
void input_destroy(Input* input);
void input_update(Input* input);
int input_key_down(Input* input, Key key);
int input_key_up(Input* input, Key key);
int input_key_pressed(Input* input, Key key);
float input_mouse_x(Input* input);
float input_mouse_y(Input* input);
float input_mouse_dx(Input* input);
float input_mouse_dy(Input* input);
int input_mouse_down(Input* input, int button);
int input_mouse_pressed(Input* input, int button);

void input_set_key(Input* input, int key, int state);

void input_set_mouse(Input* input, float x, float y);
void input_set_mouse_delta(Input* input, float dx, float dy);
void input_set_mouse_button(Input* input, int button, int state);

#ifdef __cplusplus
}
#endif

#endif /* INPUT_H */
