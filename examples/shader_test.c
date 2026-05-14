#include "engine.h"
#include "resource/loader.h"
#include "core/math.h"

int main(void) {
    log_init(NULL);

    Engine* engine = engine_create(1280, 720, "Shader File Test");
    if (!engine) {
        log_error("Failed to create engine");
        return 1;
    }

    char* vpath = asset_path("assets/shaders/basic.vert");
    char* fpath = asset_path("assets/shaders/basic.frag");
    Shader* shader = shader_create_file(vpath, fpath);
    free(vpath);
    free(fpath);
    if (!shader) {
        log_error("Failed to load shader files");
        engine_destroy(engine);
        return 1;
    }

    renderer_use_shader(engine->renderer, shader);
    engine->renderer->clear_color[0] = 0.1f;
    engine->renderer->clear_color[1] = 0.15f;
    engine->renderer->clear_color[2] = 0.2f;
    renderer_enable_depth(engine->renderer);

    Vertex vertices[] = {
        {-0.5f, -0.5f, 0.0f, 0.0f, 0.0f},
        { 0.5f, -0.5f, 0.0f, 1.0f, 0.0f},
        { 0.0f,  0.5f, 0.0f, 0.5f, 1.0f},
    };
    GLuint indices[] = {0, 1, 2};
    Mesh* triangle = mesh_create(vertices, indices, 3, 3);

    camera_set_position(engine->camera, vec3(0, 0, 2));
    camera_set_target(engine->camera, vec3(0, 0, 0));

    log_info("Running shader file test...");

    while (engine_running(engine)) {
        engine_update(engine);

        if (input_key_down(engine->input, KEY_ESCAPE)) {
            engine->running = 0;
        }

        shader_use(shader);
        shader_set_vec3(shader, "uColor", 1.0f, 0.5f, 0.3f);
        shader_set_int(shader, "uUseTexture", 0);

        Mat4 model;
        mat4_identity(model);
        shader_set_mat4(shader, "uModel", (GLfloat*)&model);

        renderer_draw_mesh(engine->renderer, triangle, engine->camera);
        engine_swap(engine);
    }

    mesh_destroy(triangle);
    shader_destroy(shader);
    engine_destroy(engine);
    log_info("Engine shutdown");
    return 0;
}
