#include "rendering/camera.h"
#include <stdlib.h>
#include <string.h>

Camera* camera_create(Vec3 position, float fov, float aspect, float near_plane, float far_plane) {
    Camera* cam = malloc(sizeof(Camera));
    cam->position = position;
    cam->front = vec3(0, 0, -1);
    cam->up = vec3(0, 1, 0);
    cam->world_up = vec3(0, 1, 0);
    cam->right = vec3(1, 0, 0);
    cam->yaw = -90.0f;
    cam->pitch = 0.0f;
    cam->fov = fov;
    cam->near_plane = near_plane;
    cam->far_plane = far_plane;
    cam->aspect = aspect;
    cam->projection = CAMERA_PERSPECTIVE;
    return cam;
}

void camera_destroy(Camera* cam) {
    if (cam) free(cam);
}

void camera_update(Camera* cam, float aspect) {
    cam->aspect = aspect;
    cam->front.x = cosf(radians(cam->yaw)) * cosf(radians(cam->pitch));
    cam->front.y = sinf(radians(cam->pitch));
    cam->front.z = sinf(radians(cam->yaw)) * cosf(radians(cam->pitch));
    cam->front = vec3_normalize(cam->front);
    cam->right = vec3_normalize(vec3_cross(cam->front, cam->world_up));
    cam->up = vec3_normalize(vec3_cross(cam->right, cam->front));

    mat4_look_at(cam->view, cam->position, vec3_add(cam->position, cam->front), cam->up);
    mat4_perspective(cam->projection_mat, radians(cam->fov), cam->aspect, cam->near_plane, cam->far_plane);
}

void camera_get_view(Camera* cam, Mat4* out) {
    memcpy(out, cam->view, sizeof(Mat4));
}

void camera_get_projection(Camera* cam, Mat4* out) {
    memcpy(out, cam->projection_mat, sizeof(Mat4));
}

void camera_process_mouse(Camera* cam, float x_offset, float y_offset) {
    cam->yaw += x_offset * 0.1f;
    cam->pitch += y_offset * 0.1f;
    if (cam->pitch > 89.9f) cam->pitch = 89.9f;
    if (cam->pitch < -89.9f) cam->pitch = -89.9f;
}

void camera_process_scroll(Camera* cam, float y_offset) {
    cam->fov -= y_offset;
    if (cam->fov < 1.0f) cam->fov = 1.0f;
    if (cam->fov > 90.0f) cam->fov = 90.0f;
}

void camera_set_position(Camera* cam, Vec3 pos) {
    cam->position = pos;
}

void camera_set_target(Camera* cam, Vec3 target) {
    cam->front = vec3_normalize(vec3_sub(target, cam->position));
    cam->yaw = degrees(atan2f(cam->front.z, cam->front.x));
    cam->pitch = degrees(asinf(cam->front.y));
}
