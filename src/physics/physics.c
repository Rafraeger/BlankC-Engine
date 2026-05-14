#include "physics/physics.h"
#include "core/log.h"
#include <stdlib.h>
#include <string.h>

PhysicsWorld* physics_world_create(void) {
    PhysicsWorld* world = malloc(sizeof(PhysicsWorld));
    world->capacity = 32;
    world->body_count = 0;
    world->bodies = malloc(sizeof(RigidBody*) * world->capacity);
    world->gravity = vec3(0.0f, -9.81f, 0.0f);
    return world;
}

void physics_world_destroy(PhysicsWorld* world) {
    if (!world) return;
    for (int i = 0; i < world->body_count; i++) {
        free(world->bodies[i]);
    }
    free(world->bodies);
    free(world);
}

RigidBody* physics_world_add_body(PhysicsWorld* world, Vec3 position, Vec3 size, int is_static) {
    if (world->body_count >= world->capacity) {
        world->capacity *= 2;
        world->bodies = realloc(world->bodies, sizeof(RigidBody*) * world->capacity);
    }

    RigidBody* body = malloc(sizeof(RigidBody));
    body->position = position;
    body->velocity = vec3(0, 0, 0);
    body->size = size;
    body->is_static = is_static;
    body->user_data = NULL;

    world->bodies[world->body_count++] = body;
    return body;
}

void physics_world_remove_body(PhysicsWorld* world, RigidBody* body) {
    for (int i = 0; i < world->body_count; i++) {
        if (world->bodies[i] == body) {
            free(world->bodies[i]);
            world->bodies[i] = world->bodies[--world->body_count];
            return;
        }
    }
}

AABB rigidbody_get_aabb(RigidBody* body) {
    AABB aabb;
    Vec3 half = vec3_scale(body->size, 0.5f);
    aabb.min = vec3_sub(body->position, half);
    aabb.max = vec3_add(body->position, half);
    return aabb;
}

static int test_aabb_overlap(AABB a, AABB b, Vec3* mtv) {
    float dx = (a.max.x + a.min.x) / 2.0f - (b.max.x + b.min.x) / 2.0f;
    float dy = (a.max.y + a.min.y) / 2.0f - (b.max.y + b.min.y) / 2.0f;
    float dz = (a.max.z + a.min.z) / 2.0f - (b.max.z + b.min.z) / 2.0f;

    float width_a = a.max.x - a.min.x;
    float width_b = b.max.x - b.min.x;
    float overlap_x = (width_a + width_b) / 2.0f - fabsf(dx);
    if (overlap_x <= 0) return 0;

    float height_a = a.max.y - a.min.y;
    float height_b = b.max.y - b.min.y;
    float overlap_y = (height_a + height_b) / 2.0f - fabsf(dy);
    if (overlap_y <= 0) return 0;

    float depth_a = a.max.z - a.min.z;
    float depth_b = b.max.z - b.min.z;
    float overlap_z = (depth_a + depth_b) / 2.0f - fabsf(dz);
    if (overlap_z <= 0) return 0;

    // Smallest overlap determines the MTV (Minimum Translation Vector)
    if (overlap_x < overlap_y && overlap_x < overlap_z) {
        *mtv = vec3((dx > 0 ? 1.0f : -1.0f) * overlap_x, 0, 0);
    } else if (overlap_y < overlap_x && overlap_y < overlap_z) {
        *mtv = vec3(0, (dy > 0 ? 1.0f : -1.0f) * overlap_y, 0);
    } else {
        *mtv = vec3(0, 0, (dz > 0 ? 1.0f : -1.0f) * overlap_z);
    }

    return 1;
}

void physics_world_update(PhysicsWorld* world, float delta_time) {
    for (int i = 0; i < world->body_count; i++) {
        RigidBody* a = world->bodies[i];
        if (a->is_static) continue;

        // Apply simple gravity
        a->velocity = vec3_add(a->velocity, vec3_scale(world->gravity, delta_time));

        // Integrate position
        a->position = vec3_add(a->position, vec3_scale(a->velocity, delta_time));


        // Resolve collisions against all other bodies (mostly static ones)
        for (int j = 0; j < world->body_count; j++) {
            if (i == j) continue;
            RigidBody* b = world->bodies[j];

            AABB aabb_a = rigidbody_get_aabb(a);
            AABB aabb_b = rigidbody_get_aabb(b);

            Vec3 mtv;
            if (test_aabb_overlap(aabb_a, aabb_b, &mtv)) {
                // If collision detected, push A out of B
                // For a simple world, we only push dynamic objects out of static ones
                // or dynamic out of dynamic (half each).
                if (b->is_static) {
                    a->position = vec3_add(a->position, mtv);
                    
                    // Zero out velocity on the collision axis to stop jitter
                    if (mtv.x != 0) a->velocity.x = 0;
                    if (mtv.y != 0) a->velocity.y = 0;
                    if (mtv.z != 0) a->velocity.z = 0;
                } else {
                    // Both dynamic, push both half-way
                    Vec3 half_mtv = vec3_scale(mtv, 0.5f);
                    a->position = vec3_add(a->position, half_mtv);
                    b->position = vec3_sub(b->position, half_mtv);
                }
            }
        }
    }
}
