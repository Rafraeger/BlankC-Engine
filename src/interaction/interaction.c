#include "interaction/interaction.h"
#include <stdlib.h>
#include <float.h>

struct InteractionSystem {
    SceneGraph* scene;
    Camera* camera;
};

InteractionSystem* interaction_init(SceneGraph* scene, Camera* camera) {
    InteractionSystem* sys = malloc(sizeof(InteractionSystem));
    if (!sys) return NULL;
    sys->scene = scene;
    sys->camera = camera;
    return sys;
}

void interaction_shutdown(InteractionSystem* sys) {
    if (sys) free(sys);
}

// Fungsi internal rekursif untuk mencari node terdekat
static void find_closest_node(SceneNode* node, Ray ray, float max_dist, SceneNode** closest_node, float* min_dist) {
    if (!node) return;

    // Use world-space position from the world matrix for correct hit testing
    // even when the node is a child with a parent transform.
    AABB box;
    Vec3 pos = transform_get_world_position(node->transform);
    Vec3 scale = node->transform->scale;
    
    // TODO: For full accuracy, scale should also be composed from parent transforms.
    // This works correctly for root-level nodes which is the common case.
    box.min = vec3(pos.x - scale.x * 0.5f, pos.y - scale.y * 0.5f, pos.z - scale.z * 0.5f);
    box.max = vec3(pos.x + scale.x * 0.5f, pos.y + scale.y * 0.5f, pos.z + scale.z * 0.5f);

    float d;
    if (math_ray_aabb_intersect(ray, box, &d)) {
        if (d >= 0 && d <= max_dist && d < *min_dist) {
            *min_dist = d;
            *closest_node = node;
        }
    }

    // Periksa anak-anaknya
    for (int i = 0; i < node->child_count; i++) {
        find_closest_node(node->children[i], ray, max_dist, closest_node, min_dist);
    }
}

SceneNode* interaction_query(InteractionSystem* sys, float max_distance, float* out_distance) {
    if (!sys || !sys->camera || !sys->scene) return NULL;

    Ray ray;
    ray.origin = sys->camera->position;
    ray.direction = sys->camera->front;

    SceneNode* closest = NULL;
    float min_d = FLT_MAX;

    find_closest_node(scenegraph_root(sys->scene), ray, max_distance, &closest, &min_d);

    if (out_distance) *out_distance = min_d;
    return closest;
}
