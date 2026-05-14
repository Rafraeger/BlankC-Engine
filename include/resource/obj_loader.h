#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H

#include "rendering/mesh.h"
#include "rendering/texture.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    Mesh* mesh;
    Texture* texture;
    char* path;
} ObjModel;

ObjModel* obj_load(const char* path, const char* texture_path);
void obj_render(ObjModel* model);
void obj_destroy(ObjModel* model);

#ifdef __cplusplus
}
#endif

#endif /* OBJ_LOADER_H */
