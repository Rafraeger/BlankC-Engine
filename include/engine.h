#ifndef ENGINE_H
#define ENGINE_H

#include "core/log.h"
#include "core/math.h"
#include "core/memory.h"
#include "core/time.h"
#include "rendering/gl_context.h"
#include "rendering/shader.h"
#include "rendering/mesh.h"
#include "rendering/camera.h"
#include "rendering/renderer.h"
#include "input/input.h"
#include "scene/transform.h"
#include "scene/scenegraph.h"
#include "resource/loader.h"
#include "physics/physics.h"
#include "ui/ui.h"
#include "audio/audio.h"
#include "interaction/interaction.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Engine {
    GLContext* context;
    Input* input;
    Camera* camera;
    Renderer* renderer;
    SceneGraph* scene;
    PhysicsWorld* physics;
    UI* ui;
    AudioSystem* audio;
    InteractionSystem* interaction;
    float delta_time;
    int running;
} Engine;




Engine* engine_create(int width, int height, const char* title);
void engine_update(Engine* engine);
void engine_swap(Engine* engine);
void engine_destroy(Engine* engine);
int engine_running(Engine* engine);

#ifdef __cplusplus
}
#endif

#endif /* ENGINE_H */
