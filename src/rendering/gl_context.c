#include "rendering/gl_context.h"
#include "glad.h"
#include "core/log.h"
#include <GLFW/glfw3.h>
#include <stdlib.h>

static void error_callback(int error, const char* description) {
    log_error("GLFW error %d: %s", error, description);
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

static void* glad_glfw_loader(const char* name) {
    return (void*)glfwGetProcAddress(name);
}

GLContext* gl_context_create(int width, int height, const char* title) {
    glfwSetErrorCallback(error_callback);
    if (!glfwInit()) {
        log_error("gl_context_create: glfwInit failed");
        return NULL;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!window) {
        log_error("gl_context_create: glfwCreateWindow failed");
        glfwTerminate();
        return NULL;
    }

    glfwMakeContextCurrent(window);
    
    if (!gladLoadGL(glad_glfw_loader)) {
        log_error("gl_context_create: gladLoadGL failed");
        glfwDestroyWindow(window);
        glfwTerminate();
        return NULL;
    }

    glfwSwapInterval(1);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    GLContext* ctx = malloc(sizeof(GLContext));
    ctx->window = window;
    ctx->width = width;
    ctx->height = height;
    ctx->running = 1;
    return ctx;
}


void gl_context_resize(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void gl_context_update(GLContext* ctx) {
    if (glfwWindowShouldClose(ctx->window)) {
        ctx->running = 0;
    }
    glfwPollEvents();
    /* Ensure internal width/height stay in sync with actual framebuffer size for aspect ratio calculations */
    glfwGetFramebufferSize(ctx->window, &ctx->width, &ctx->height);
}

int gl_context_running(GLContext* ctx) {
    return ctx->running;
}

void gl_context_swap(GLContext* ctx) {
    glfwSwapBuffers(ctx->window);
}

void gl_context_lock_mouse(GLContext* ctx, int locked) {
    if (locked) {
        glfwSetInputMode(ctx->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    } else {
        glfwSetInputMode(ctx->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

GLFWwindow* gl_context_get_window(GLContext* ctx) {
    return ctx->window;
}

void gl_context_destroy(GLContext* ctx) {
    if (ctx) {
        glfwDestroyWindow(ctx->window);
        glfwTerminate();
        free(ctx);
    }
}
