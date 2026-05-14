#ifndef INTERACTION_H
#define INTERACTION_H

#include "rendering/camera.h"
#include "scene/scenegraph.h"
#include "core/math.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct InteractionSystem InteractionSystem;

InteractionSystem* interaction_init(SceneGraph* scene, Camera* camera);
void interaction_shutdown(InteractionSystem* sys);

// Mencari node terdekat yang terkena ray pandangan kamera
SceneNode* interaction_query(InteractionSystem* sys, float max_distance, float* out_distance);

#ifdef __cplusplus
}
#endif

#endif /* INTERACTION_H */
