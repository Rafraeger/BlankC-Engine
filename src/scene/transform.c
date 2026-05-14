#include "scene/transform.h"
#include <stdlib.h>
#include <string.h>

Transform* transform_create(void) {
    Transform* t = malloc(sizeof(Transform));
    t->position = vec3(0, 0, 0);
    t->rotation = vec3(0, 0, 0);
    t->scale = vec3(1, 1, 1);
    t->parent = NULL;
    memset(t->local_matrix, 0, sizeof(Mat4));
    memset(t->world_matrix, 0, sizeof(Mat4));
    return t;
}

void transform_destroy(Transform* t) {
    if (t) free(t);
}

void transform_set_position(Transform* t, Vec3 pos) {
    t->position = pos;
}

void transform_set_rotation(Transform* t, Vec3 rot) {
    t->rotation = rot;
}

void transform_set_scale(Transform* t, Vec3 s) {
    t->scale = s;
}

void transform_update(Transform* t) {
    Mat4 translation, rot_x, rot_y, rot_z, scale;
    mat4_translate(translation, t->position.x, t->position.y, t->position.z);
    mat4_rotate_x(rot_x, radians(t->rotation.x));
    mat4_rotate_y(rot_y, radians(t->rotation.y));
    mat4_rotate_z(rot_z, radians(t->rotation.z));
    mat4_scale(scale, t->scale.x, t->scale.y, t->scale.z);

    Mat4 rot, temp_rot, local, temp_local;
    mat4_multiply(temp_rot, rot_y, rot_z);
    mat4_multiply(rot, rot_x, temp_rot);
    mat4_multiply(temp_local, translation, rot);
    mat4_multiply(local, temp_local, scale);
    memcpy(t->local_matrix, local, sizeof(Mat4));

    if (t->parent) {
        Mat4 world;
        mat4_multiply(world, t->parent->world_matrix, t->local_matrix);
        memcpy(t->world_matrix, world, sizeof(Mat4));
    } else {
        memcpy(t->world_matrix, t->local_matrix, sizeof(Mat4));
    }
}

Vec3 transform_get_position(Transform* t) {
    return t->position;
}

Vec3 transform_get_world_position(Transform* t) {
    /* Extract translation from column-major world matrix (indices 12,13,14) */
    return vec3(t->world_matrix[12], t->world_matrix[13], t->world_matrix[14]);
}

void transform_get_world_matrix(Transform* t, Mat4 out) {
    memcpy(out, t->world_matrix, sizeof(Mat4));
}
