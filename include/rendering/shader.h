#ifndef SHADER_H
#define SHADER_H

#include "glad.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    GLuint id;
    GLuint program;
} Shader;

Shader* shader_create(const char* vert_src, const char* frag_src);
Shader* shader_create_file(const char* vert_path, const char* frag_path);
void shader_use(Shader* shader);
void shader_destroy(Shader* shader);
void shader_set_int(Shader* shader, const char* name, GLint val);
void shader_set_float(Shader* shader, const char* name, GLfloat val);
void shader_set_vec3(Shader* shader, const char* name, GLfloat x, GLfloat y, GLfloat z);
void shader_set_mat4(Shader* shader, const char* name, const GLfloat* mat);

#ifdef __cplusplus
}
#endif

#endif /* SHADER_H */
