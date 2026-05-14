#ifndef RENDERER_H
#define RENDERER_H

#include "rendering/shader.h"
#include "rendering/mesh.h"
#include "rendering/camera.h"
#include "scene/scenegraph.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    Shader* active_shader;
    float clear_color[4];
    int depth_test_enabled;
} Renderer;

Renderer* renderer_create(void);
void renderer_destroy(Renderer* renderer);
void renderer_clear(Renderer* renderer);
void renderer_set_clear_color(Renderer* renderer, float r, float g, float b, float a);
void renderer_use_shader(Renderer* renderer, Shader* shader);
void renderer_draw_mesh(Renderer* renderer, Mesh* mesh, Camera* camera);
void renderer_draw_scene(Renderer* renderer, SceneGraph* graph, Camera* camera);
void renderer_enable_depth(Renderer* renderer);
void renderer_disable_depth(Renderer* renderer);

#ifdef __cplusplus
}
#endif

#endif /* RENDERER_H */
