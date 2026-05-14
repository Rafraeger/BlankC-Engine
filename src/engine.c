#include "engine.h"
#include "core/log.h"
#include "rendering/gl_defines.h"
#include <stdlib.h>

Engine* engine_create(int width, int height, const char* title) {
    Engine* engine = malloc(sizeof(Engine));
    if (!engine) {
        log_error("engine_create: malloc failed");
        return NULL;
    }
    log_info("engine_create: allocated Engine");

    engine->context = gl_context_create(width, height, title);
    log_info("engine_create: gl_context_create returned %p", (void*)engine->context);
    if (!engine->context) {
        log_error("engine_create: gl_context_create returned NULL");
        free(engine);
        return NULL;
    }

    log_info("engine_create: about to input_create");
    engine->input = input_create();
    log_info("engine_create: about to camera_create");
    engine->camera = camera_create(vec3(0, 1, 3), 45.0f, (float)width / height, 0.1f, 100.0f);
    log_info("engine_create: about to renderer_create");
    engine->renderer = renderer_create();
    log_info("engine_create: about to scenegraph_create");
    engine->scene = scenegraph_create();
    log_info("engine_create: about to physics_world_create");
    engine->physics = physics_world_create();
    log_info("engine_create: about to ui_init");
    engine->ui = ui_init(gl_context_get_window(engine->context));
    log_info("engine_create: about to audio_init");
    engine->audio = audio_init();
    log_info("engine_create: about to interaction_init");
    engine->interaction = interaction_init(engine->scene, engine->camera);
    engine->delta_time = 0;
    engine->running = 1;




    log_info("engine_create: about to time_init");
    time_init();
    log_info("Engine created: %s (%dx%d)", title, width, height);
    return engine;
}

static float s_last_mx = 0, s_last_my = 0;
static int s_first_mouse = 1;

static void engine_poll_input(Engine* engine) {
    GLFWwindow* win = engine->context->window;

    /* Poll keyboard */
    for (int k = 32; k <= 348; k++) {
        input_set_key(engine->input, k, glfwGetKey(win, k) == GLFW_PRESS);
    }

    /* Poll mouse position & compute delta */
    double mx, my;
    glfwGetCursorPos(win, &mx, &my);

    if (s_first_mouse) {
        s_last_mx = (float)mx;
        s_last_my = (float)my;
        s_first_mouse = 0;
    }

    float dx = (float)mx - s_last_mx;
    float dy = s_last_my - (float)my; /* Y inverted: up = positive */
    s_last_mx = (float)mx;
    s_last_my = (float)my;

    input_set_mouse(engine->input, (float)mx, (float)my);
    input_set_mouse_delta(engine->input, dx, dy);

    /* Poll mouse buttons */
    input_set_mouse_button(engine->input, 0,
        glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);
    input_set_mouse_button(engine->input, 1,
        glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS);
}

void engine_update(Engine* engine) {
    time_tick();
    engine->delta_time = time_delta();

    gl_context_update(engine->context);
    ui_new_frame(engine->ui);
    input_update(engine->input);
    engine_poll_input(engine);
    camera_update(engine->camera, (float)engine->context->width / engine->context->height);
    scenegraph_update(engine->scene);
    physics_world_update(engine->physics, engine->delta_time);

    glDisable(GL_SCISSOR_TEST);
    glDisable(GL_BLEND);

    renderer_clear(engine->renderer);

    engine->running = gl_context_running(engine->context);
}

void engine_swap(Engine* engine) {
    ui_render(engine->ui);
    gl_context_swap(engine->context);
}

void engine_destroy(Engine* engine) {
    if (engine) {
        interaction_shutdown(engine->interaction);
        audio_shutdown(engine->audio);
        ui_shutdown(engine->ui);
        physics_world_destroy(engine->physics);
        scenegraph_destroy(engine->scene);
        renderer_destroy(engine->renderer);
        camera_destroy(engine->camera);
        input_destroy(engine->input);
        log_shutdown();
        /* GL context must be destroyed LAST so all GL cleanup calls above are valid */
        gl_context_destroy(engine->context);

        free(engine);
    }
}



int engine_running(Engine* engine) {
    return engine->running;
}
