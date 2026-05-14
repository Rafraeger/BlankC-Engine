#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "core/math.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Transform {
    Vec3 position;
    Vec3 rotation;
    Vec3 scale;
    Mat4 local_matrix;
    Mat4 world_matrix;
    struct Transform* parent;
} Transform;

Transform* transform_create(void);
void transform_destroy(Transform* t);
void transform_set_position(Transform* t, Vec3 pos);
void transform_set_rotation(Transform* t, Vec3 rot);
void transform_set_scale(Transform* t, Vec3 scale);
void transform_update(Transform* t);
Vec3 transform_get_position(Transform* t);
Vec3 transform_get_world_position(Transform* t);
void transform_get_world_matrix(Transform* t, Mat4 out);

#ifdef __cplusplus
}
#endif

#endif /* TRANSFORM_H */
