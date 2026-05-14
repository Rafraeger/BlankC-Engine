#ifndef MATH_H
#define MATH_H

#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

typedef struct { float x, y; } Vec2;
typedef struct { float x, y, z; } Vec3;
typedef struct { float x, y, z, w; } Vec4;
typedef float Mat4[16];

typedef struct {
    Vec3 origin;
    Vec3 direction;
} Ray;

typedef struct {
    Vec3 min;
    Vec3 max;
} AABB;

Vec2 vec2(float x, float y);
Vec3 vec3(float x, float y, float z);
Vec4 vec4(float x, float y, float z, float w);

Vec3 vec3_add(Vec3 a, Vec3 b);
Vec3 vec3_sub(Vec3 a, Vec3 b);
Vec3 vec3_scale(Vec3 v, float s);
Vec3 vec3_mul(Vec3 a, Vec3 b);
float vec3_dot(Vec3 a, Vec3 b);
Vec3 vec3_cross(Vec3 a, Vec3 b);
float vec3_length(Vec3 v);
Vec3 vec3_normalize(Vec3 v);
Vec3 vec3_lerp(Vec3 a, Vec3 b, float t);

void mat4_identity(Mat4 out);
void mat4_multiply(Mat4 out, Mat4 a, Mat4 b);
void mat4_translate(Mat4 out, float x, float y, float z);
void mat4_rotate_x(Mat4 out, float angle);
void mat4_rotate_y(Mat4 out, float angle);
void mat4_rotate_z(Mat4 out, float angle);
void mat4_scale(Mat4 out, float sx, float sy, float sz);
void mat4_perspective(Mat4 out, float fov, float aspect, float near, float far);
void mat4_look_at(Mat4 out, Vec3 eye, Vec3 center, Vec3 up);
void mat4_transpose(Mat4 out, Mat4 m);
Vec4 mat4_mul_vec4(Mat4 m, Vec4 v);

float radians(float degrees);
float degrees(float radians);

/* Raycasting */
int math_ray_aabb_intersect(Ray ray, AABB aabb, float* t_out);

#ifdef __cplusplus
}
#endif

#endif /* MATH_H */
