#include "camera_controller.h"
#include "resource/loader.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>


CameraController* controller_create(Camera* camera, Input* input, AudioSystem* audio) {
    CameraController* ctrl = malloc(sizeof(CameraController));
    ctrl->camera = camera;
    ctrl->input = input;
    ctrl->audio = audio;
    ctrl->body = NULL;
    ctrl->move_speed = 5.0f;
    ctrl->mouse_sensitivity = 0.1f;
    ctrl->mouse_locked = 0;
    return ctrl;
}



void controller_destroy(CameraController* ctrl) {
    if (ctrl) free(ctrl);
}

void controller_set_move_speed(CameraController* ctrl, float speed) {
    ctrl->move_speed = speed;
}

void controller_set_sensitivity(CameraController* ctrl, float sens) {
    ctrl->mouse_sensitivity = sens;
}

void controller_update(CameraController* ctrl, float delta_time) {
    Camera* cam = ctrl->camera;
    Input* inp = ctrl->input;

    if (!cam || !inp) return;

    // Handle mouse rotation regardless of physics
    float dx = input_mouse_dx(inp);
    float dy = input_mouse_dy(inp);
    if (dx != 0.0f || dy != 0.0f) {
        camera_process_mouse(cam, dx * ctrl->mouse_sensitivity, dy * ctrl->mouse_sensitivity);
    }

    if (input_key_down(inp, KEY_LEFT_CONTROL)) {
        camera_process_scroll(cam, delta_time * 10.0f);
    }

    // Movement
    if (ctrl->body) {
        // Physics-based movement
        Vec3 forward = cam->front;
        forward.y = 0; // Keep movement on the ground plane
        forward = vec3_normalize(forward);

        Vec3 right = cam->right;
        
        Vec3 wish_dir = vec3(0, 0, 0);
        if (input_key_down(inp, KEY_W)) wish_dir = vec3_add(wish_dir, forward);
        if (input_key_down(inp, KEY_S)) wish_dir = vec3_sub(wish_dir, forward);
        if (input_key_down(inp, KEY_A)) wish_dir = vec3_sub(wish_dir, right);
        if (input_key_down(inp, KEY_D)) wish_dir = vec3_add(wish_dir, right);

        if (vec3_length(wish_dir) > 0) {
            wish_dir = vec3_normalize(wish_dir);
        }

        // Set velocity based on input
        float current_y_vel = ctrl->body->velocity.y; // Preserve gravity/vertical velocity
        ctrl->body->velocity = vec3_scale(wish_dir, ctrl->move_speed);
        ctrl->body->velocity.y = current_y_vel;

        // Jump (very simple)
        if (input_key_down(inp, KEY_SPACE) && fabsf(ctrl->body->velocity.y) < 0.01f) {
            ctrl->body->velocity.y = 5.0f;
            
            /* Play jump sound */
            if (ctrl->audio) {
                char* jump_path = asset_path("assets/audio/jump.wav");
                audio_play_sound(ctrl->audio, jump_path);
                free(jump_path);
            }
        }

        // Sync camera to physics body
        cam->position = ctrl->body->position;
        // Offset camera height slightly (eye level)
        cam->position.y += 1.5f; 
    } else {
        // Fallback to noclip (old behavior)
        float speed = ctrl->move_speed * delta_time;
        Vec3 forward = cam->front;
        Vec3 right = cam->right;

        if (input_key_down(inp, KEY_W)) cam->position = vec3_add(cam->position, vec3_scale(forward, speed));
        if (input_key_down(inp, KEY_S)) cam->position = vec3_add(cam->position, vec3_scale(forward, -speed));
        if (input_key_down(inp, KEY_A)) cam->position = vec3_add(cam->position, vec3_scale(right, -speed));
        if (input_key_down(inp, KEY_D)) cam->position = vec3_add(cam->position, vec3_scale(right, speed));
        if (input_key_down(inp, KEY_SPACE)) cam->position.y += speed;
        if (input_key_down(inp, KEY_LEFT_SHIFT)) cam->position.y -= speed;
    }
}

