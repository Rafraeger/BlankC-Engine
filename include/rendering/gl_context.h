#ifndef GL_CONTEXT_H
#define GL_CONTEXT_H

#define GLFW_INCLUDE_NONE
#include "glad.h"
#include <GLFW/glfw3.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct GLContext {
    GLFWwindow* window;
    int width;
    int height;
    int running;
} GLContext;

GLContext* gl_context_create(int width, int height, const char* title);
void gl_context_resize(GLFWwindow* window, int width, int height);
void gl_context_update(GLContext* ctx);
int gl_context_running(GLContext* ctx);
void gl_context_swap(GLContext* ctx);
void gl_context_lock_mouse(GLContext* ctx, int locked);
GLFWwindow* gl_context_get_window(GLContext* ctx);
void gl_context_destroy(GLContext* ctx);

#ifdef __cplusplus
}
#endif

#endif /* GL_CONTEXT_H */
