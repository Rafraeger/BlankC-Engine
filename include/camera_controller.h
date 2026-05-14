#ifndef CAMERA_CONTROLLER_H
#define CAMERA_CONTROLLER_H

#include "rendering/camera.h"
#include "input/input.h"
#include "physics/physics.h"
#include "audio/audio.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    Camera* camera;
    Input* input;
    RigidBody* body;
    AudioSystem* audio; // New: audio system for triggers
    float move_speed;
    float mouse_sensitivity;
    int mouse_locked;
} CameraController;

CameraController* controller_create(Camera* camera, Input* input, AudioSystem* audio);

void controller_destroy(CameraController* ctrl);
void controller_update(CameraController* ctrl, float delta_time);
void controller_set_move_speed(CameraController* ctrl, float speed);
void controller_set_sensitivity(CameraController* ctrl, float sens);

#ifdef __cplusplus
}
#endif

#endif /* CAMERA_CONTROLLER_H */
