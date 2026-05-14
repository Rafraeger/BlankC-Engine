#include "engine.h"
#include "resource/obj_loader.h"
#include "resource/loader.h"
#include "scene/scenegraph.h"
#include "camera_controller.h"
#include "ui/ui.h"
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#include "nuklear.h"

#define TITLE_FMT "%s (%.1f FPS)"


static void setup_fps_display(Engine* engine, float fps) {
    char buf[128];
    snprintf(buf, sizeof(buf), TITLE_FMT, "Room Demo", fps);
    glfwSetWindowTitle(gl_context_get_window(engine->context), buf);
}

static void on_cube_interact(SceneNode* node, void* engine_ptr) {
    Engine* engine = (Engine*)engine_ptr;
    log_info("Interaction triggered on: %s!", node->name);
    
    /* Change color or play sound */
    audio_play_sound(engine->audio, asset_path("assets/audio/jump.wav"));
    
    /* Just a simple visual change: move it up slightly */
    node->transform->position.y += 0.5f;
}

int main(void) {
    log_init(NULL);

    Engine* engine = engine_create(1280, 720, "Room Demo");
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
    renderer_set_clear_color(engine->renderer, 0.05f, 0.05f, 0.1f, 1.0f);
    renderer_enable_depth(engine->renderer);

    /* Play background music */
    char* bgm_path = asset_path("assets/audio/bgm.mp3");
    audio_play_music(engine->audio, bgm_path);
    free(bgm_path);

    /* FPS camera controller */
    CameraController* ctrl = controller_create(engine->camera, engine->input, engine->audio);
    controller_set_move_speed(ctrl, 8.0f);
    controller_set_sensitivity(ctrl, 0.15f);

    /* Lock mouse cursor for FPS control */
    gl_context_lock_mouse(engine->context, 1);

    /* Camera position */
    camera_set_position(engine->camera, vec3(0, 1.5f, 5));

    /* Physics Setup */
    // Player body (dynamic)
    RigidBody* player_body = physics_world_add_body(engine->physics, vec3(0, 1.0f, 5), vec3(0.6f, 1.8f, 0.6f), 0);
    ctrl->body = player_body;

    /* Load OBJ model */
    char* model_path = asset_path("assets/models/cube.obj");
    ObjModel* cube_model = obj_load(model_path, NULL);
    free(model_path);
    if (!cube_model) {
        log_error("Failed to load cube model");
        return 1;
    }

    /* Load textures */
    char* tp;
    Texture* tex_checker = texture_create_file(tp = asset_path("assets/textures/checkerboard.png")); free(tp);
    Texture* tex_red = texture_create_file(tp = asset_path("assets/textures/red.png")); free(tp);
    Texture* tex_green = texture_create_file(tp = asset_path("assets/textures/green.png")); free(tp);
    Texture* tex_blue = texture_create_file(tp = asset_path("assets/textures/blue.png")); free(tp);
    Texture* tex_yellow = texture_create_file(tp = asset_path("assets/textures/yellow.png")); free(tp);

    /* Build scene */
    SceneGraph* scene = engine->scene;
    SceneNode* root = scenegraph_root(scene);

    /* Floor - 1 unit thick, center at y=-0.5 */
    SceneNode* floor_node = scenegraph_add_node(scene, root, "Floor");
    floor_node->user_data = cube_model->mesh;
    transform_set_position(floor_node->transform, vec3(0, -0.5f, 0));
    transform_set_scale(floor_node->transform, vec3(20, 1.0f, 20));
    physics_world_add_body(engine->physics, vec3(0, -0.5f, 0), vec3(20, 1.0f, 20), 1);

    /* Back wall */
    SceneNode* back_wall = scenegraph_add_node(scene, root, "BackWall");
    back_wall->user_data = cube_model->mesh;
    transform_set_position(back_wall->transform, vec3(0, 1.5f, -10));
    transform_set_scale(back_wall->transform, vec3(20, 4, 0.5f));
    physics_world_add_body(engine->physics, vec3(0, 1.5f, -10), vec3(20, 4, 0.5f), 1);

    /* Left wall */
    SceneNode* left_wall = scenegraph_add_node(scene, root, "LeftWall");
    left_wall->user_data = cube_model->mesh;
    transform_set_position(left_wall->transform, vec3(-10, 1.5f, 0));
    transform_set_scale(left_wall->transform, vec3(0.5f, 4, 20));
    physics_world_add_body(engine->physics, vec3(-10, 1.5f, 0), vec3(0.5f, 4, 20), 1);

    /* Right wall */
    SceneNode* right_wall = scenegraph_add_node(scene, root, "RightWall");
    right_wall->user_data = cube_model->mesh;
    transform_set_position(right_wall->transform, vec3(10, 1.5f, 0));
    transform_set_scale(right_wall->transform, vec3(0.5f, 4, 20));
    physics_world_add_body(engine->physics, vec3(10, 1.5f, 0), vec3(0.5f, 4, 20), 1);

    /* Colored cubes with physics */
    SceneNode* red_cube = scenegraph_add_node(scene, root, "RedCube");
    red_cube->user_data = cube_model->mesh;
    transform_set_position(red_cube->transform, vec3(-2, 0.0f, -2));
    physics_world_add_body(engine->physics, vec3(-2, 0.0f, -2), vec3(1, 1, 1), 1);
    
    /* Make RedCube interactive */
    red_cube->is_interactive = 1;
    red_cube->on_interact = on_cube_interact;

    SceneNode* green_cube = scenegraph_add_node(scene, root, "GreenCube");
    green_cube->user_data = cube_model->mesh;
    transform_set_position(green_cube->transform, vec3(0, 0.0f, -2));
    physics_world_add_body(engine->physics, vec3(0, 0.0f, -2), vec3(1, 1, 1), 1);

    SceneNode* blue_cube = scenegraph_add_node(scene, root, "BlueCube");
    blue_cube->user_data = cube_model->mesh;
    transform_set_position(blue_cube->transform, vec3(2, 0.0f, -2));
    physics_world_add_body(engine->physics, vec3(2, 0.0f, -2), vec3(1, 1, 1), 1);

    SceneNode* yellow_cube = scenegraph_add_node(scene, root, "YellowCube");
    yellow_cube->user_data = cube_model->mesh;
    transform_set_position(yellow_cube->transform, vec3(0, 0.0f, -3.5));
    physics_world_add_body(engine->physics, vec3(0, 0.0f, -3.5), vec3(1, 1, 1), 1);


    /* Bigger cube in center */
    SceneNode* center_cube = scenegraph_add_node(engine->scene, root, "CenterCube");
    center_cube->user_data = cube_model->mesh;
    transform_set_position(center_cube->transform, vec3(0, -0.4f, 0));
    transform_set_scale(center_cube->transform, vec3(1.5f, 1.5f, 1.5f));

    /* FPS tracking */
    float fps = 0.0f;
    int frame_count = 0;
    float fps_timer = 0.0f;

    /* Light properties for UI control */
    float light_pos[3] = {0.0f, 3.0f, 0.0f};
    float light_color[3] = {1.0f, 1.0f, 1.0f};

    log_info("Room demo running - WASD to move, mouse to look, ESC to quit/unlock mouse, click to lock");

    while (engine_running(engine)) {
        engine_update(engine);

        if (input_key_down(engine->input, KEY_ESCAPE)) {
            gl_context_lock_mouse(engine->context, 0);
            ctrl->mouse_locked = 0;
        }
        if (input_mouse_down(engine->input, 0) && !ui_want_capture_mouse(engine->ui)) {
            gl_context_lock_mouse(engine->context, 1);
            ctrl->mouse_locked = 1;
        }

        /* Update FPS camera controller ONLY if mouse is locked */
        if (ctrl->mouse_locked) {
            controller_update(ctrl, engine->delta_time);
        }

        /* UI Window */
        struct nk_context* ctx = engine->ui->ctx;
        int win_w = engine->context->width;
        int win_h = engine->context->height;

        /* Draw HUD / Crosshair (Transparent overlay window) */
        if (nk_begin(ctx, "HUD", nk_rect(0, 0, (float)win_w, (float)win_h), 
            NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_BACKGROUND | NK_WINDOW_NOT_INTERACTIVE)) 
        {
            struct nk_command_buffer* canvas = nk_window_get_canvas(ctx);
            
            float look_dist;
            SceneNode* look_node = interaction_query(engine->interaction, 3.0f, &look_dist);
            struct nk_color cross_color = nk_rgb(255, 255, 255);

            if (look_node && look_node->is_interactive) {
                cross_color = nk_rgb(255, 255, 0); // Yellow when interactive
                nk_draw_text(canvas, nk_rect(win_w/2.0f + 20, win_h/2.0f - 10, 200, 20), 
                    "Press [E] to Interact", 21, ctx->style.font, nk_rgb(0,0,0), nk_rgb(255,255,0));
            }
            
            nk_fill_rect(canvas, nk_rect(win_w/2.0f - 2, win_h/2.0f - 2, 4, 4), 0, cross_color);
        }
        nk_end(ctx);

        if (nk_begin(ctx, "Engine Control", nk_rect(10, 10, 230, 250),
            NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE|
            NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE))
        {
            nk_layout_row_dynamic(ctx, 20, 1);
            nk_label(ctx, "Lighting Settings:", NK_TEXT_LEFT);

            nk_layout_row_dynamic(ctx, 25, 1);
            nk_property_float(ctx, "Light X", -10.0f, &light_pos[0], 10.0f, 0.1f, 0.1f);
            nk_property_float(ctx, "Light Y", 0.0f, &light_pos[1], 10.0f, 0.1f, 0.1f);
            nk_property_float(ctx, "Light Z", -10.0f, &light_pos[2], 10.0f, 0.1f, 0.1f);

            nk_layout_row_dynamic(ctx, 20, 1);
            nk_label(ctx, "Light Color:", NK_TEXT_LEFT);
            nk_layout_row_dynamic(ctx, 25, 3);
            nk_property_float(ctx, "R", 0.0f, &light_color[0], 1.0f, 0.01f, 0.01f);
            nk_property_float(ctx, "G", 0.0f, &light_color[1], 1.0f, 0.01f, 0.01f);
            nk_property_float(ctx, "B", 0.0f, &light_color[2], 1.0f, 0.01f, 0.01f);

            nk_layout_row_dynamic(ctx, 20, 1);
            char fps_label[32];
            snprintf(fps_label, sizeof(fps_label), "FPS: %.1f", fps);
            nk_label(ctx, fps_label, NK_TEXT_LEFT);
        }
        nk_end(ctx);

        /* FPS counter */
        frame_count++;
        fps_timer += engine->delta_time;
        if (fps_timer >= 1.0f) {
            fps = (float)frame_count;
            frame_count = 0;
            fps_timer = 0.0f;
            setup_fps_display(engine, fps);
        }

        /* Render */
        shader_use(shader);

        // Set lighting uniforms from UI
        shader_set_vec3(shader, "uLightPos", light_pos[0], light_pos[1], light_pos[2]);
        shader_set_vec3(shader, "uLightColor", light_color[0], light_color[1], light_color[2]);
        shader_set_vec3(shader, "uViewPos", engine->camera->position.x, 
                                            engine->camera->position.y, 
                                            engine->camera->position.z);

        renderer_draw_scene(engine->renderer, engine->scene, engine->camera);

        engine_swap(engine);
    }


    /* Cleanup */
    gl_context_lock_mouse(engine->context, 0);
    obj_destroy(cube_model);
    if (tex_checker) texture_destroy(tex_checker);
    if (tex_red) texture_destroy(tex_red);
    if (tex_green) texture_destroy(tex_green);
    if (tex_blue) texture_destroy(tex_blue);
    if (tex_yellow) texture_destroy(tex_yellow);
    controller_destroy(ctrl);
    shader_destroy(shader);
    engine_destroy(engine);
    log_info("Engine shutdown");
    return 0;
}
