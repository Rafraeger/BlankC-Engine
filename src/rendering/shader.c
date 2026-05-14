#include "rendering/shader.h"
#include "resource/loader.h"
#include "core/log.h"
#include <stdlib.h>
#include <string.h>

static GLuint compile_shader(GLenum type, const char* src) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char log[512];
        glGetShaderInfoLog(shader, 512, NULL, log);
        log_error("Shader compilation failed: %s", log);
    }
    return shader;
}

Shader* shader_create(const char* vert_src, const char* frag_src) {
    Shader* shader = malloc(sizeof(Shader));

    GLuint vert = compile_shader(GL_VERTEX_SHADER, vert_src);
    GLuint frag = compile_shader(GL_FRAGMENT_SHADER, frag_src);

    shader->program = glCreateProgram();
    glAttachShader(shader->program, vert);
    glAttachShader(shader->program, frag);
    glLinkProgram(shader->program);

    int success;
    glGetProgramiv(shader->program, GL_LINK_STATUS, &success);
    if (!success) {
        char log[512];
        glGetProgramInfoLog(shader->program, 512, NULL, log);
        log_error("Shader linking failed: %s", log);
    }

    glDeleteShader(vert);
    glDeleteShader(frag);
    shader->id = shader->program;
    return shader;
}

Shader* shader_create_file(const char* vert_path, const char* frag_path) {
    if (!vert_path || !frag_path) return NULL;
    long v_size = 0, f_size = 0;
    char* v_src = file_read_text(vert_path, &v_size);
    char* f_src = file_read_text(frag_path, &f_size);

    if (!v_src || !f_src) {
        if (v_src) file_text_free(v_src);
        if (f_src) file_text_free(f_src);
        return NULL;
    }

    Shader* shader = shader_create(v_src, f_src);
    file_text_free(v_src);
    file_text_free(f_src);
    return shader;
}

void shader_use(Shader* shader) {
    glUseProgram(shader->program);
}

void shader_destroy(Shader* shader) {
    if (shader) {
        glDeleteProgram(shader->program);
        free(shader);
    }
}

void shader_set_int(Shader* shader, const char* name, GLint val) {
    glUseProgram(shader->program);
    glUniform1i(glGetUniformLocation(shader->program, name), val);
}

void shader_set_float(Shader* shader, const char* name, GLfloat val) {
    glUseProgram(shader->program);
    glUniform1f(glGetUniformLocation(shader->program, name), val);
}

void shader_set_vec3(Shader* shader, const char* name, GLfloat x, GLfloat y, GLfloat z) {
    glUseProgram(shader->program);
    glUniform3f(glGetUniformLocation(shader->program, name), x, y, z);
}

void shader_set_mat4(Shader* shader, const char* name, const GLfloat* mat) {
    glUseProgram(shader->program);
    glUniformMatrix4fv(glGetUniformLocation(shader->program, name), 1, GL_FALSE, mat);
}
