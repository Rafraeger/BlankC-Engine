#ifndef MESH_H
#define MESH_H

#include "glad.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
    GLuint count;
} Mesh;

typedef struct {
    float x, y, z;
    float u, v;
    float nx, ny, nz;
} Vertex;

Mesh* mesh_create(Vertex* vertices, GLuint* indices, GLuint vert_count, GLuint index_count);
void mesh_render(Mesh* mesh);
void mesh_destroy(Mesh* mesh);

#ifdef __cplusplus
}
#endif

#endif /* MESH_H */
