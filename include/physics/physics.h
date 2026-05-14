#ifndef PHYSICS_H
#define PHYSICS_H

#include "core/math.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct RigidBody {
    Vec3 position;
    Vec3 velocity;
    Vec3 size;      // Full dimensions (width, height, depth)
    int is_static;  // 1 if it doesn't move (e.g., walls), 0 if it does (e.g., player)
    void* user_data;
} RigidBody;

typedef struct PhysicsWorld {
    RigidBody** bodies;
    int body_count;
    int capacity;
    Vec3 gravity;
} PhysicsWorld;

PhysicsWorld* physics_world_create(void);
void physics_world_destroy(PhysicsWorld* world);

RigidBody* physics_world_add_body(PhysicsWorld* world, Vec3 position, Vec3 size, int is_static);
void physics_world_remove_body(PhysicsWorld* world, RigidBody* body);

// Update all bodies and resolve collisions
void physics_world_update(PhysicsWorld* world, float delta_time);

// Helper to get current AABB of a body
AABB rigidbody_get_aabb(RigidBody* body);

#ifdef __cplusplus
}
#endif

#endif /* PHYSICS_H */
