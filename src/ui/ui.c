#include "ui/ui.h"
#include "glad.h"
#include "rendering/gl_defines.h"
#include <stdlib.h>


#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_GLFW_GL3_IMPLEMENTATION
#define NK_KEYSTATE_BASED_INPUT

#include "nuklear.h"
#include "nuklear_glfw_gl3.h"

UI* ui_init(GLFWwindow* window) {
    UI* ui = malloc(sizeof(UI));
    ui->glfw = malloc(sizeof(struct nk_glfw));
    ui->ctx = nk_glfw3_init(ui->glfw, window, NK_GLFW3_INSTALL_CALLBACKS);
    
    struct nk_font_atlas *atlas;
    nk_glfw3_font_stash_begin(ui->glfw, &atlas);
    nk_glfw3_font_stash_end(ui->glfw);
    
    return ui;
}

void ui_new_frame(UI* ui) {
    nk_glfw3_new_frame(ui->glfw);
}

void ui_render(UI* ui) {
    nk_glfw3_render(ui->glfw, NK_ANTI_ALIASING_ON, 512 * 1024, 128 * 1024);
}

void ui_shutdown(UI* ui) {
    if (ui) {
        nk_glfw3_shutdown(ui->glfw);
        free(ui->glfw);
        free(ui);
    }
}

int ui_want_capture_mouse(UI* ui) {
    return nk_window_is_any_hovered(ui->ctx);
}

int ui_want_capture_keyboard(UI* ui) {
    return nk_item_is_any_active(ui->ctx);
}
