#ifndef UI_H
#define UI_H

#include "rendering/gl_context.h"

// Forward declarations
struct nk_context;
struct nk_glfw;

typedef struct UI {
    struct nk_context* ctx;
    struct nk_glfw* glfw;
} UI;


UI* ui_init(GLFWwindow* window);
void ui_new_frame(UI* ui);
void ui_render(UI* ui);
void ui_shutdown(UI* ui);

// Check if UI wants to capture mouse/keyboard
int ui_want_capture_mouse(UI* ui);
int ui_want_capture_keyboard(UI* ui);

#endif /* UI_H */
