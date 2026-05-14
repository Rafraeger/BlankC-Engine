#include "core/math.h"
#include <string.h>

Vec2 vec2(float x, float y) { Vec2 v = {x, y}; return v; }
Vec3 vec3(float x, float y, float z) { Vec3 v = {x, y, z}; return v; }
Vec4 vec4(float x, float y, float z, float w) { Vec4 v = {x, y, z, w}; return v; }

Vec3 vec3_add(Vec3 a, Vec3 b) { return vec3(a.x + b.x, a.y + b.y, a.z + b.z); }
Vec3 vec3_sub(Vec3 a, Vec3 b) { return vec3(a.x - b.x, a.y - b.y, a.z - b.z); }
Vec3 vec3_scale(Vec3 v, float s) { return vec3(v.x * s, v.y * s, v.z * s); }
Vec3 vec3_mul(Vec3 a, Vec3 b) { return vec3(a.x * b.x, a.y * b.y, a.z * b.z); }

float vec3_dot(Vec3 a, Vec3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vec3 vec3_cross(Vec3 a, Vec3 b) {
    return vec3(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    );
}

float vec3_length(Vec3 v) {
    return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

Vec3 vec3_normalize(Vec3 v) {
    float len = vec3_length(v);
    if (len > 0.0001f) {
        return vec3_scale(v, 1.0f / len);
    }
    return vec3(0, 0, 0);
}

Vec3 vec3_lerp(Vec3 a, Vec3 b, float t) {
    return vec3(
        a.x + (b.x - a.x) * t,
        a.y + (b.y - a.y) * t,
        a.z + (b.z - a.z) * t
    );
}

void mat4_identity(Mat4 out) {
    memset(out, 0, sizeof(Mat4));
    out[0] = 1; out[5] = 1; out[10] = 1; out[15] = 1;
}

void mat4_multiply(Mat4 out, Mat4 a, Mat4 b) {
    Mat4 tmp;
    for (int i = 0; i < 4; i++) { // column of out
        for (int j = 0; j < 4; j++) { // row of out
            tmp[i * 4 + j] =
                a[0 * 4 + j] * b[i * 4 + 0] +
                a[1 * 4 + j] * b[i * 4 + 1] +
                a[2 * 4 + j] * b[i * 4 + 2] +
                a[3 * 4 + j] * b[i * 4 + 3];
        }
    }
    memcpy(out, tmp, sizeof(Mat4));
}

void mat4_translate(Mat4 out, float x, float y, float z) {
    mat4_identity(out);
    out[12] = x; out[13] = y; out[14] = z;
}

void mat4_rotate_x(Mat4 out, float angle) {
    mat4_identity(out);
    float c = cosf(angle), s = sinf(angle);
    out[5] = c; out[6] = s;
    out[9] = -s; out[10] = c;
}

void mat4_rotate_y(Mat4 out, float angle) {
    mat4_identity(out);
    float c = cosf(angle), s = sinf(angle);
    out[0] = c; out[2] = -s;
    out[8] = s; out[10] = c;
}

void mat4_rotate_z(Mat4 out, float angle) {
    mat4_identity(out);
    float c = cosf(angle), s = sinf(angle);
    out[0] = c; out[1] = s;
    out[4] = -s; out[5] = c;
}

void mat4_scale(Mat4 out, float sx, float sy, float sz) {
    mat4_identity(out);
    out[0] = sx; out[5] = sy; out[10] = sz;
}

void mat4_perspective(Mat4 out, float fov, float aspect, float near, float far) {
    memset(out, 0, sizeof(Mat4));
    float f = 1.0f / tanf(fov / 2.0f);
    out[0] = f / aspect;
    out[5] = f;
    out[10] = (far + near) / (near - far);
    out[11] = -1.0f;
    out[14] = (2.0f * far * near) / (near - far);
}

void mat4_look_at(Mat4 out, Vec3 eye, Vec3 center, Vec3 up) {
    Vec3 f = vec3_normalize(vec3_sub(center, eye));
    Vec3 r = vec3_normalize(vec3_cross(f, up));
    Vec3 u = vec3_cross(r, f);

    mat4_identity(out);
    out[0] = r.x; out[1] = u.x; out[2] = -f.x;
    out[4] = r.y; out[5] = u.y; out[6] = -f.y;
    out[8] = r.z; out[9] = u.z; out[10] = -f.z;
    out[12] = -vec3_dot(r, eye);
    out[13] = -vec3_dot(u, eye);
    out[14] = vec3_dot(f, eye);
}

void mat4_transpose(Mat4 out, Mat4 m) {
    Mat4 tmp;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            tmp[i * 4 + j] = m[j * 4 + i];
    memcpy(out, tmp, sizeof(Mat4));
}

Vec4 mat4_mul_vec4(Mat4 m, Vec4 v) {
    return vec4(
        m[0] * v.x + m[4] * v.y + m[8] * v.z + m[12] * v.w,
        m[1] * v.x + m[5] * v.y + m[9] * v.z + m[13] * v.w,
        m[2] * v.x + m[6] * v.y + m[10] * v.z + m[14] * v.w,
        m[3] * v.x + m[7] * v.y + m[11] * v.z + m[15] * v.w
    );
}

float radians(float degrees) { return degrees * M_PI / 180.0f; }
float degrees(float rad) { return rad * 180.0f / M_PI; }

int math_ray_aabb_intersect(Ray ray, AABB aabb, float* t_out) {
    float tmin = -INFINITY;
    float tmax = INFINITY;

    float origin[3] = {ray.origin.x, ray.origin.y, ray.origin.z};
    float dir[3] = {ray.direction.x, ray.direction.y, ray.direction.z};
    float min[3] = {aabb.min.x, aabb.min.y, aabb.min.z};
    float max[3] = {aabb.max.x, aabb.max.y, aabb.max.z};

    for (int i = 0; i < 3; i++) {
        if (fabsf(dir[i]) > 1e-6f) {
            float t1 = (min[i] - origin[i]) / dir[i];
            float t2 = (max[i] - origin[i]) / dir[i];

            if (t1 > t2) { float tmp = t1; t1 = t2; t2 = tmp; }
            if (t1 > tmin) tmin = t1;
            if (t2 < tmax) tmax = t2;

            if (tmin > tmax) return 0;
        } else {
            if (origin[i] < min[i] || origin[i] > max[i]) return 0;
        }
    }

    if (tmax < 0) return 0;
    
    if (t_out) *t_out = (tmin < 0) ? tmax : tmin;
    return 1;
}
