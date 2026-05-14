#include "engine.h"

static const char* vert_src =
    "#version 330 core\n"
    "layout(location = 0) in vec3 aPos;\n"
    "layout(location = 1) in vec2 aTexCoord;\n"
    "out vec2 TexCoord;\n"
    "uniform mat4 uModel;\n"
    "uniform mat4 uView;\n"
    "uniform mat4 uProjection;\n"
    "void main() {\n"
    "    gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0);\n"
    "    TexCoord = aTexCoord;\n"
    "}\n";

static const char* frag_src =
    "#version 330 core\n"
    "in vec2 TexCoord;\n"
    "out vec4 FragColor;\n"
    "void main() {\n"
    "    FragColor = vec4(1.0, 0.33, 0.33, 1.0);\n"
    "}\n";

int main(void) {
    log_init(NULL);

    Engine* engine = engine_create(1280, 720, "Triangle Example");
    if (!engine) {
        log_error("Failed to create engine");
        return 1;
    }

    Shader* shader = shader_create(vert_src, frag_src);
    renderer_use_shader(engine->renderer, shader);
    engine->renderer->clear_color[0] = 0.1f;
    engine->renderer->clear_color[1] = 0.15f;
    engine->renderer->clear_color[2] = 0.2f;
    renderer_enable_depth(engine->renderer);

    Vertex vertices[] = {
        {-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f},
        { 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f},
        { 0.0f,  0.5f, 0.0f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f},
    };
    GLuint indices[] = {0, 1, 2};
    Mesh* triangle = mesh_create(vertices, indices, 3, 3);

    camera_set_position(engine->camera, vec3(0, 0, 2));
    camera_set_target(engine->camera, vec3(0, 0, 0));

    log_info("Running triangle example...");

    while (engine_running(engine)) {
        engine_update(engine);

        if (input_key_down(engine->input, KEY_ESCAPE)) {
            engine->running = 0;
        }

        shader_use(shader);
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
