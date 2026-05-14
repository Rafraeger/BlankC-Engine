#define NOGDI
#include "input/input.h"
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <string.h>

Input* input_create(void) {
    Input* input = malloc(sizeof(Input));
    memset(input->keys, 0, sizeof(input->keys));
    memset(input->prev_keys, 0, sizeof(input->prev_keys));
    input->mouse_x = 0;
    input->mouse_y = 0;
    input->dx = 0;
    input->dy = 0;
    input->mouse_left = 0;
    input->mouse_right = 0;
    return input;
}

void input_destroy(Input* input) {
    if (input) free(input);
}

void input_update(Input* input) {
    memcpy(input->prev_keys, input->keys, sizeof(input->keys));
    input->prev_mouse_left = input->mouse_left;
    input->prev_mouse_right = input->mouse_right;
    input->dx = 0;
    input->dy = 0;
}

int input_key_down(Input* input, Key key) {
    return input->keys[key];
}

int input_key_up(Input* input, Key key) {
    return !input->keys[key];
}

int input_key_pressed(Input* input, Key key) {
    return input->keys[key] && !input->prev_keys[key];
}

float input_mouse_x(Input* input) {
    return input->mouse_x;
}

float input_mouse_y(Input* input) {
    return input->mouse_y;
}

float input_mouse_dx(Input* input) {
    return input->dx;
}

float input_mouse_dy(Input* input) {
    return input->dy;
}

int input_mouse_down(Input* input, int button) {
    if (button == 0) return input->mouse_left;
    if (button == 1) return input->mouse_right;
    return 0;
}

int input_mouse_pressed(Input* input, int button) {
    if (button == 0) return input->mouse_left && !input->prev_mouse_left;
    if (button == 1) return input->mouse_right && !input->prev_mouse_right;
    return 0;
}

void input_set_key(Input* input, int key, int state) {
    if (key < 512) input->keys[key] = state;
}

void input_set_mouse(Input* input, float x, float y) {
    input->mouse_x = x;
    input->mouse_y = y;
}

void input_set_mouse_delta(Input* input, float dx, float dy) {
    input->dx += dx;
    input->dy += dy;
}

void input_set_mouse_button(Input* input, int button, int state) {
    if (button == 0) input->mouse_left = state;
    if (button == 1) input->mouse_right = state;
}
