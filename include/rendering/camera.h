#ifndef CAMERA_H
#define CAMERA_H

#include "core/math.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    CAMERA_PERSPECTIVE,
    CAMERA_ORTHOGRAPHIC
} CameraProjection;

typedef struct {
    Vec3 position;
    Vec3 front;
    Vec3 up;
    Vec3 right;
    Vec3 world_up;
    float yaw;
    float pitch;
    float fov;
    float near_plane;
    float far_plane;
    float aspect;
    CameraProjection projection;
    Mat4 view;
    Mat4 projection_mat;
} Camera;

Camera* camera_create(Vec3 position, float fov, float aspect, float near_plane, float far_plane);
void camera_destroy(Camera* cam);
void camera_update(Camera* cam, float aspect);
void camera_get_view(Camera* cam, Mat4* out);
void camera_get_projection(Camera* cam, Mat4* out);
void camera_process_mouse(Camera* cam, float x_offset, float y_offset);
void camera_process_scroll(Camera* cam, float y_offset);
void camera_set_position(Camera* cam, Vec3 pos);
void camera_set_target(Camera* cam, Vec3 target);

#ifdef __cplusplus
}
#endif

#endif /* CAMERA_H */
