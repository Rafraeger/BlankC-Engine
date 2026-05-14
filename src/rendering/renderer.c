#include "rendering/renderer.h"
#include <stdlib.h>

Renderer* renderer_create(void) {
    Renderer* r = malloc(sizeof(Renderer));
    r->active_shader = NULL;
    r->clear_color[0] = 0;
    r->clear_color[1] = 0;
    r->clear_color[2] = 0;
    r->clear_color[3] = 1;
    r->depth_test_enabled = 0;
    return r;
}

void renderer_destroy(Renderer* renderer) {
    if (renderer) free(renderer);
}

void renderer_clear(Renderer* renderer) {
    glClearColor(
        renderer->clear_color[0],
        renderer->clear_color[1],
        renderer->clear_color[2],
        renderer->clear_color[3]
    );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void renderer_set_clear_color(Renderer* renderer, float r, float g, float b, float a) {
    renderer->clear_color[0] = r;
    renderer->clear_color[1] = g;
    renderer->clear_color[2] = b;
    renderer->clear_color[3] = a;
}

void renderer_use_shader(Renderer* renderer, Shader* shader) {
    renderer->active_shader = shader;
    shader_use(shader);
}

void renderer_draw_mesh(Renderer* renderer, Mesh* mesh, Camera* camera) {
    if (renderer->active_shader) {
        Mat4 view, proj;
        camera_get_view(camera, &view);
        camera_get_projection(camera, &proj);
        shader_set_mat4(renderer->active_shader, "uView", (GLfloat*)&view);
        shader_set_mat4(renderer->active_shader, "uProjection", (GLfloat*)&proj);
    }
    mesh_render(mesh);
}

static void renderer_draw_node(Renderer* renderer, SceneNode* node, Camera* camera) {
    if (!node || !node->transform) return;

    if (node->user_data) {
        Mesh* mesh = (Mesh*)node->user_data;
        shader_set_mat4(renderer->active_shader, "uModel",
                        (GLfloat*)&node->transform->world_matrix);
        
        if (node->texture) {
            shader_set_int(renderer->active_shader, "uUseTexture", 1);
            texture_bind(node->texture, 0);
            shader_set_int(renderer->active_shader, "uTexture", 0);
        } else {
            shader_set_int(renderer->active_shader, "uUseTexture", 0);
            shader_set_vec3(renderer->active_shader, "uColor", 1.0f, 1.0f, 1.0f);
        }

        mesh_render(mesh);
    }

    for (int i = 0; i < node->child_count; i++) {
        renderer_draw_node(renderer, node->children[i], camera);
    }
}

void renderer_draw_scene(Renderer* renderer, SceneGraph* graph, Camera* camera) {
    if (!graph || !graph->root) return;

    if (renderer->active_shader) {
        Mat4 view, proj;
        camera_get_view(camera, &view);
        camera_get_projection(camera, &proj);
        shader_set_mat4(renderer->active_shader, "uView", (GLfloat*)&view);
        shader_set_mat4(renderer->active_shader, "uProjection", (GLfloat*)&proj);
    }

    renderer_draw_node(renderer, graph->root, camera);
}

void renderer_enable_depth(Renderer* renderer) {
    glEnable(GL_DEPTH_TEST);
    renderer->depth_test_enabled = 1;
}

void renderer_disable_depth(Renderer* renderer) {
    glDisable(GL_DEPTH_TEST);
    renderer->depth_test_enabled = 0;
}
