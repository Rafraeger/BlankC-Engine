#include "engine.h"
#include "resource/obj_loader.h"
#include "resource/loader.h"
#include "scene/scenegraph.h"

int main(void) {
    log_init(NULL);

    Engine* engine = engine_create(1280, 720, "OBJ Demo");
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
        log_error("Failed to load shaders");
        return 1;
    }
    renderer_use_shader(engine->renderer, shader);
    renderer_set_clear_color(engine->renderer, 0.1f, 0.15f, 0.2f, 1.0f);
    renderer_enable_depth(engine->renderer);

    /* Load OBJ model */
    char* model_path = asset_path("assets/models/cube.obj");
    ObjModel* model = obj_load(model_path, NULL);
    free(model_path);
    if (!model) {
        log_error("Failed to load OBJ model");
        return 1;
    }

    /* Scene graph */
    SceneGraph* scene = scenegraph_create();
    SceneNode* cube_node = scenegraph_add_node(scene, scenegraph_root(scene), "Cube");
    cube_node->user_data = model->mesh;

    /* Camera setup */
    camera_set_position(engine->camera, vec3(0, 1, 3));
    camera_set_target(engine->camera, vec3(0, 0, 0));

    log_info("Running OBJ demo...");

    float angle = 0.0f;
    while (engine_running(engine)) {
        engine_update(engine);

        if (input_key_down(engine->input, KEY_ESCAPE))
            engine->running = 0;

        angle += 0.01f;
        transform_set_rotation(cube_node->transform, vec3(0, angle, 0));
        scenegraph_update(scene);

        shader_use(shader);
        renderer_draw_scene(engine->renderer, scene, engine->camera);

        engine_swap(engine);
    }

    obj_destroy(model);
    scenegraph_destroy(scene);
    shader_destroy(shader);
    engine_destroy(engine);
    log_info("Engine shutdown");
    return 0;
}
