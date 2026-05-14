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


#define MAX_ENEMIES 10

typedef struct {
    SceneNode* node;
    RigidBody* body;
    float health;
    float speed;
    int active;
} Enemy;

typedef enum {
    STATE_MENU,
    STATE_PLAYING,
    STATE_GAMEOVER,
    STATE_WIN
} GameState;

static GameState g_state = STATE_MENU;

typedef struct {
    float health;
    float max_health;
    int ammo;
    int max_ammo;
} PlayerStats;

static PlayerStats g_player = {100.0f, 100.0f, 30, 30};

static Enemy g_enemies[MAX_ENEMIES];
static int g_enemy_count = 0;

static void enemy_spawn(Engine* engine, Vec3 position, ObjModel* model, Texture* enemy_tex);

static void game_reset(Engine* engine, ObjModel* cube_model, Texture* enemy_tex, CameraController* ctrl) {
    g_player.health = g_player.max_health;
    g_player.ammo = g_player.max_ammo;
    g_state = STATE_PLAYING;
    
    renderer_enable_depth(engine->renderer); // Ensure depth is on
    
    // Clear existing enemies
    for (int i = 0; i < g_enemy_count; i++) {
        if (g_enemies[i].active) {
            physics_world_remove_body(engine->physics, g_enemies[i].body);
            g_enemies[i].body = NULL;
        }
        /* Remove enemy node from scene graph to prevent ghost rendering */
        if (g_enemies[i].node) {
            scenegraph_remove_node(engine->scene, g_enemies[i].node);
            g_enemies[i].node = NULL;
        }
    }
    g_enemy_count = 0;

    // Respawn enemies
    enemy_spawn(engine, vec3(-2, 1.0f, 25), cube_model, enemy_tex);
    enemy_spawn(engine, vec3(2, 1.0f, 25), cube_model, enemy_tex);
    enemy_spawn(engine, vec3(0, 1.0f, 28), cube_model, enemy_tex);

    // Reset player physics body and camera
    if (ctrl && ctrl->body) {
        ctrl->body->position = vec3(0, 1.0f, 5);
        ctrl->body->velocity = vec3(0, 0, 0);
    }
    camera_set_position(engine->camera, vec3(0, 1.5f, 5));
    
    gl_context_lock_mouse(engine->context, 1);
    ctrl->mouse_locked = 1;
}

static void enemy_spawn(Engine* engine, Vec3 position, ObjModel* model, Texture* enemy_tex) {
    if (g_enemy_count >= MAX_ENEMIES) return;

    Enemy* e = &g_enemies[g_enemy_count++];
    char name[32];
    snprintf(name, sizeof(name), "Enemy_%d", g_enemy_count);

    e->node = scenegraph_add_node(engine->scene, scenegraph_root(engine->scene), name);
    e->node->user_data = model->mesh;
    e->node->texture = enemy_tex;

    transform_set_position(e->node->transform, position);
    transform_set_scale(e->node->transform, vec3(0.8f, 1.8f, 0.8f));

    e->body = physics_world_add_body(engine->physics, position, vec3(0.8f, 1.8f, 0.8f), 0);
    e->body->user_data = e; // Store pointer back to Enemy

    e->health = 50.0f;
    e->speed = 3.5f;
    e->active = 1;
}

static void enemy_update_all(Engine* engine) {
    int active_enemies = 0;
    for (int i = 0; i < g_enemy_count; i++) {
        Enemy* e = &g_enemies[i];
        if (!e->active) continue;
        active_enemies++;

        // Simple AI: Move towards player
        Vec3 player_pos = engine->camera->position;
        Vec3 enemy_pos = e->body->position;
        
        Vec3 dir = vec3_sub(player_pos, enemy_pos);
        dir.y = 0; // Stay on ground
        
        float dist = vec3_length(dir);
        if (dist > 1.5f && dist < 15.0f) {
            dir = vec3_normalize(dir);
            e->body->velocity.x = dir.x * e->speed;
            e->body->velocity.z = dir.z * e->speed;
        } else {
            e->body->velocity.x = 0;
            e->body->velocity.z = 0;
            
            // Damage player if too close
            if (dist <= 1.5f) {
                g_player.health -= 15.0f * engine->delta_time;
                if (g_player.health <= 0) {
                    g_player.health = 0;
                    g_state = STATE_GAMEOVER;
                    gl_context_lock_mouse(engine->context, 0);
                }
            }
        }

        // Sync node to body
        transform_set_position(e->node->transform, e->body->position);
    }

    if (active_enemies == 0 && g_enemy_count > 0) {
        g_state = STATE_WIN;
        gl_context_lock_mouse(engine->context, 0);
    }
}

static void setup_fps_display(Engine* engine, float fps) {
    char buf[128];
    snprintf(buf, sizeof(buf), TITLE_FMT, "Room Demo", fps);
    glfwSetWindowTitle(gl_context_get_window(engine->context), buf);
}

static void on_cube_interact(SceneNode* node, void* engine_ptr) {
    Engine* engine = (Engine*)engine_ptr;
    log_info("Interaction triggered on: %s!", node->name);
    
    /* Change color or play sound */
    char* snd_path = asset_path("assets/audio/jump.wav");
    audio_play_sound(engine->audio, snd_path);
    free(snd_path);
    
    /* Just a simple visual change: move it up slightly */
    node->transform->position.y += 0.5f;
}

static void on_shoot(Engine* engine) {
    if (g_player.ammo <= 0) {
        // Maybe play a click sound
        return;
    }

    g_player.ammo--;

    /* Play gunshot sound */
    char* snd_path = asset_path("assets/audio/jump.wav");
    audio_play_sound(engine->audio, snd_path);
    free(snd_path);

    float dist;
    SceneNode* hit = interaction_query(engine->interaction, 50.0f, &dist);
    
    if (hit) {
        log_info("Hit object: %s at distance %.2f", hit->name, dist);
        
        /* Check if we hit an enemy */
        for (int i = 0; i < g_enemy_count; i++) {
            if (g_enemies[i].active && g_enemies[i].node == hit) {
                g_enemies[i].health -= 25.0f;
                log_info("Enemy hit! Health: %.1f", g_enemies[i].health);
                
                if (g_enemies[i].health <= 0) {
                    g_enemies[i].active = 0;
                    g_enemies[i].health = 0;
                    transform_set_scale(g_enemies[i].node->transform, vec3(0,0,0)); // Hide
                    physics_world_remove_body(engine->physics, g_enemies[i].body);
                    g_enemies[i].body = NULL; /* Prevent use-after-free */
                    log_info("Enemy killed!");
                }
                return;
            }
        }

        if (hit->is_interactive && hit->on_interact) {
            hit->on_interact(hit, engine);
        }
    }
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
    renderer_set_clear_color(engine->renderer, 0.1f, 0.1f, 0.2f, 1.0f); // Darker blue background
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

    /* Build scene - FPS Level */
    SceneGraph* scene = engine->scene;
    SceneNode* root = scenegraph_root(scene);

    /* Main Room Floor */
    SceneNode* floor_node = scenegraph_add_node(scene, root, "Floor");
    floor_node->user_data = cube_model->mesh;
    floor_node->texture = tex_checker;
    transform_set_position(floor_node->transform, vec3(0, -0.5f, 0));
    transform_set_scale(floor_node->transform, vec3(20, 1.0f, 20));
    physics_world_add_body(engine->physics, vec3(0, -0.5f, 0), vec3(20, 1.0f, 20), 1);

    /* Perimeter Walls */
    // Back wall
    SceneNode* back_wall = scenegraph_add_node(scene, root, "BackWall");
    back_wall->user_data = cube_model->mesh;
    back_wall->texture = tex_blue;
    transform_set_position(back_wall->transform, vec3(0, 1.5f, -10));
    transform_set_scale(back_wall->transform, vec3(20, 4, 0.5f));
    physics_world_add_body(engine->physics, vec3(0, 1.5f, -10), vec3(20, 4, 0.5f), 1);

    // Front wall (with gap for corridor)
    SceneNode* front_wall_l = scenegraph_add_node(scene, root, "FrontWallL");
    front_wall_l->user_data = cube_model->mesh;
    front_wall_l->texture = tex_blue;
    transform_set_position(front_wall_l->transform, vec3(-6, 1.5f, 10));
    transform_set_scale(front_wall_l->transform, vec3(8, 4, 0.5f));
    physics_world_add_body(engine->physics, vec3(-6, 1.5f, 10), vec3(8, 4, 0.5f), 1);

    SceneNode* front_wall_r = scenegraph_add_node(scene, root, "FrontWallR");
    front_wall_r->user_data = cube_model->mesh;
    front_wall_r->texture = tex_blue;
    transform_set_position(front_wall_r->transform, vec3(6, 1.5f, 10));
    transform_set_scale(front_wall_r->transform, vec3(8, 4, 0.5f));
    physics_world_add_body(engine->physics, vec3(6, 1.5f, 10), vec3(8, 4, 0.5f), 1);

    // Left wall
    SceneNode* left_wall = scenegraph_add_node(scene, root, "LeftWall");
    left_wall->user_data = cube_model->mesh;
    left_wall->texture = tex_blue;
    transform_set_position(left_wall->transform, vec3(-10, 1.5f, 0));
    transform_set_scale(left_wall->transform, vec3(0.5f, 4, 20));
    physics_world_add_body(engine->physics, vec3(-10, 1.5f, 0), vec3(0.5f, 4, 20), 1);

    // Right wall
    SceneNode* right_wall = scenegraph_add_node(scene, root, "RightWall");
    right_wall->user_data = cube_model->mesh;
    right_wall->texture = tex_blue;
    transform_set_position(right_wall->transform, vec3(10, 1.5f, 0));
    transform_set_scale(right_wall->transform, vec3(0.5f, 4, 20));
    physics_world_add_body(engine->physics, vec3(10, 1.5f, 0), vec3(0.5f, 4, 20), 1);

    /* Corridor */
    SceneNode* corr_floor = scenegraph_add_node(scene, root, "CorrFloor");
    corr_floor->user_data = cube_model->mesh;
    corr_floor->texture = tex_checker;
    transform_set_position(corr_floor->transform, vec3(0, -0.5f, 15));
    transform_set_scale(corr_floor->transform, vec3(4, 1.0f, 10));
    physics_world_add_body(engine->physics, vec3(0, -0.5f, 15), vec3(4, 1.0f, 10), 1);

    SceneNode* corr_wall_l = scenegraph_add_node(scene, root, "CorrWallL");
    corr_wall_l->user_data = cube_model->mesh;
    corr_wall_l->texture = tex_blue;
    transform_set_position(corr_wall_l->transform, vec3(-2, 1.5f, 15));
    transform_set_scale(corr_wall_l->transform, vec3(0.5f, 4, 10));
    physics_world_add_body(engine->physics, vec3(-2, 1.5f, 15), vec3(0.5f, 4, 10), 1);

    SceneNode* corr_wall_r = scenegraph_add_node(scene, root, "CorrWallR");
    corr_wall_r->user_data = cube_model->mesh;
    corr_wall_r->texture = tex_blue;
    transform_set_position(corr_wall_r->transform, vec3(2, 1.5f, 15));
    transform_set_scale(corr_wall_r->transform, vec3(0.5f, 4, 10));
    physics_world_add_body(engine->physics, vec3(2, 1.5f, 15), vec3(0.5f, 4, 10), 1);

    /* Second Room */
    SceneNode* room2_floor = scenegraph_add_node(scene, root, "Room2Floor");
    room2_floor->user_data = cube_model->mesh;
    room2_floor->texture = tex_checker;
    transform_set_position(room2_floor->transform, vec3(0, -0.5f, 25));
    transform_set_scale(room2_floor->transform, vec3(10, 1.0f, 10));
    physics_world_add_body(engine->physics, vec3(0, -0.5f, 25), vec3(10, 1.0f, 10), 1);

    SceneNode* room2_back = scenegraph_add_node(scene, root, "Room2Back");
    room2_back->user_data = cube_model->mesh;
    room2_back->texture = tex_blue;
    transform_set_position(room2_back->transform, vec3(0, 1.5f, 30));
    transform_set_scale(room2_back->transform, vec3(10, 4, 0.5f));
    physics_world_add_body(engine->physics, vec3(0, 1.5f, 30), vec3(10, 4, 0.5f), 1);

    /* Obstacles in Main Room */
    SceneNode* pillar1 = scenegraph_add_node(scene, root, "Pillar1");
    pillar1->user_data = cube_model->mesh;
    pillar1->texture = tex_yellow;
    transform_set_position(pillar1->transform, vec3(-4, 1.0f, -4));
    transform_set_scale(pillar1->transform, vec3(1, 3, 1));
    physics_world_add_body(engine->physics, vec3(-4, 1.0f, -4), vec3(1, 3, 1), 1);

    SceneNode* pillar2 = scenegraph_add_node(scene, root, "Pillar2");
    pillar2->user_data = cube_model->mesh;
    pillar2->texture = tex_yellow;
    transform_set_position(pillar2->transform, vec3(4, 1.0f, -4));
    transform_set_scale(pillar2->transform, vec3(1, 3, 1));
    physics_world_add_body(engine->physics, vec3(4, 1.0f, -4), vec3(1, 3, 1), 1);

    /* Colored cubes with physics */
/* Colored cubes with physics */
SceneNode* red_cube = scenegraph_add_node(scene, root, "RedCube");
red_cube->user_data = cube_model->mesh;
red_cube->texture = tex_green;
transform_set_position(red_cube->transform, vec3(-2, 0.0f, -2));

physics_world_add_body(engine->physics, vec3(-2, 0.0f, -2), vec3(1, 1, 1), 1);

/* Make RedCube interactive */
red_cube->is_interactive = 1;
red_cube->on_interact = on_cube_interact;

/* Spawn Enemies in Second Room */
enemy_spawn(engine, vec3(-2, 1.0f, 25), cube_model, tex_red);
enemy_spawn(engine, vec3(2, 1.0f, 25), cube_model, tex_red);
enemy_spawn(engine, vec3(0, 1.0f, 28), cube_model, tex_red);

/* Bigger cube in center */
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
    float light_pos[3] = {0.0f, 10.0f, 0.0f};
    float light_color[3] = {1.0f, 1.0f, 1.0f};

    log_info("Room demo running - WASD to move, mouse to look, ESC to quit/unlock mouse, click to lock");

    while (engine_running(engine)) {
        engine_update(engine);

        struct nk_context* ctx = engine->ui->ctx;
        int win_w = engine->context->width;
        int win_h = engine->context->height;

        if (g_state == STATE_PLAYING) {
            if (input_key_down(engine->input, KEY_ESCAPE)) {
                gl_context_lock_mouse(engine->context, 0);
                ctrl->mouse_locked = 0;
            }
            if (input_mouse_down(engine->input, 0) && !ui_want_capture_mouse(engine->ui)) {
                if (!ctrl->mouse_locked) {
                    gl_context_lock_mouse(engine->context, 1);
                    ctrl->mouse_locked = 1;
                }
            }

            if (ctrl->mouse_locked) {
                controller_update(ctrl, engine->delta_time);
                enemy_update_all(engine);

                if (input_mouse_pressed(engine->input, 0)) on_shoot(engine);
                if (input_key_pressed(engine->input, KEY_R)) g_player.ammo = g_player.max_ammo;

                if (input_key_pressed(engine->input, KEY_E)) {
                    float dist;
                    SceneNode* interact_node = interaction_query(engine->interaction, 3.0f, &dist);
                    if (interact_node && interact_node->is_interactive && interact_node->on_interact) {
                        interact_node->on_interact(interact_node, engine);
                    }
                }
            }

            /* Draw HUD */
            nk_style_push_style_item(ctx, &ctx->style.window.fixed_background, nk_style_item_color(nk_rgba(0,0,0,0)));
            if (nk_begin(ctx, "HUD", nk_rect(0, 0, (float)win_w, (float)win_h), 
                NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_NO_INPUT)) 
            {
                struct nk_command_buffer* canvas = nk_window_get_canvas(ctx);
                struct nk_color cross_color = nk_rgb(255, 255, 255);
                
                float look_dist;
                SceneNode* look_node = interaction_query(engine->interaction, 3.0f, &look_dist);
                if (look_node && look_node->is_interactive) cross_color = nk_rgb(255, 255, 0);
                
                nk_fill_rect(canvas, nk_rect(win_w/2.0f - 2, win_h/2.0f - 10, 4, 20), 0, cross_color);
                nk_fill_rect(canvas, nk_rect(win_w/2.0f - 10, win_h/2.0f - 2, 20, 4), 0, cross_color);

                // Player Stats HUD
                nk_layout_row_dynamic(ctx, 30, 2);
                char hp_txt[32], am_txt[32];
                snprintf(hp_txt, sizeof(hp_txt), "HP: %.0f", g_player.health);
                snprintf(am_txt, sizeof(am_txt), "AMMO: %d/%d", g_player.ammo, g_player.max_ammo);
                nk_label(ctx, hp_txt, NK_TEXT_LEFT);
                nk_label(ctx, am_txt, NK_TEXT_RIGHT);
            }
            nk_end(ctx);
            nk_style_pop_style_item(ctx);

        } else if (g_state == STATE_MENU) {
            gl_context_lock_mouse(engine->context, 0);
            if (nk_begin(ctx, "Main Menu", nk_rect(win_w/2.0f - 100, win_h/2.0f - 75, 200, 150),
                NK_WINDOW_BORDER|NK_WINDOW_TITLE)) {
                nk_layout_row_dynamic(ctx, 40, 1);
                nk_label(ctx, "FPS ENGINE C", NK_TEXT_CENTERED);
                if (nk_button_label(ctx, "START GAME")) game_reset(engine, cube_model, tex_red, ctrl);
                if (nk_button_label(ctx, "QUIT")) engine->running = 0;
            }
            nk_end(ctx);
        } else if (g_state == STATE_GAMEOVER || g_state == STATE_WIN) {
            gl_context_lock_mouse(engine->context, 0);
            const char* msg = (g_state == STATE_WIN) ? "YOU WIN!" : "GAME OVER";
            if (nk_begin(ctx, "End Screen", nk_rect(win_w/2.0f - 100, win_h/2.0f - 75, 200, 150),
                NK_WINDOW_BORDER|NK_WINDOW_TITLE)) {
                nk_layout_row_dynamic(ctx, 40, 1);
                nk_label(ctx, msg, NK_TEXT_CENTERED);
                if (nk_button_label(ctx, "RETRY")) game_reset(engine, cube_model, tex_red, ctrl);
                if (nk_button_label(ctx, "MENU")) g_state = STATE_MENU;
            }
            nk_end(ctx);
        }

        /* Debug UI (Optional) */
        if (g_state == STATE_PLAYING && nk_begin(ctx, "Debug", nk_rect(10, 10, 230, 250), NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE|NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE)) {
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
            char fps_lbl[32]; snprintf(fps_lbl, sizeof(fps_lbl), "FPS: %.1f", fps);
            nk_label(ctx, fps_lbl, NK_TEXT_LEFT);
            nk_label(ctx, "Press [R] to Reload", NK_TEXT_LEFT);
        }
        if (g_state == STATE_PLAYING) nk_end(ctx);

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

        renderer_enable_depth(engine->renderer);
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
