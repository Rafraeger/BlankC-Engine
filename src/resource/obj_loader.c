#include "resource/obj_loader.h"
#include "core/log.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    float x, y, z;
} ObjVertex;

typedef struct {
    float u, v;
} ObjTexCoord;

typedef struct {
    float x, y, z;
} ObjNormal;

typedef struct {
    int v, vt, vn;
} ObjFaceVertex;

typedef struct {
    void* data;
    int count;
    int capacity;
    size_t elem_size;
} DynArray;

static DynArray dynarray_create(size_t elem_size, int initial_cap) {
    DynArray a;
    a.elem_size = elem_size;
    a.capacity = initial_cap;
    a.count = 0;
    a.data = malloc(elem_size * initial_cap);
    return a;
}

static void* dynarray_push(DynArray* a) {
    if (a->count >= a->capacity) {
        a->capacity *= 2;
        a->data = realloc(a->data, a->elem_size * a->capacity);
        if (!a->data) return NULL;
    }
    void* ptr = (char*)a->data + a->elem_size * a->count;
    a->count++;
    return ptr;
}

static void* dynarray_get(DynArray* a, int index) {
    return (char*)a->data + a->elem_size * index;
}

static void dynarray_free(DynArray* a) {
    free(a->data);
    a->data = NULL;
    a->count = 0;
    a->capacity = 0;
}

static int obj_parse_file(const char* path,
                          DynArray* verts, DynArray* texcoords,
                          DynArray* normals, DynArray* faces) {
    FILE* f = fopen(path, "r");
    if (!f) {
        log_error("OBJ: Cannot open file %s", path);
        return 0;
    }

    char line[256];
    while (fgets(line, sizeof(line), f)) {
        if (strncmp(line, "v ", 2) == 0) {
            ObjVertex* v = (ObjVertex*)dynarray_push(verts);
            if (!v) { fclose(f); return 0; }
            sscanf(line + 2, "%f %f %f", &v->x, &v->y, &v->z);
        } else if (strncmp(line, "vt ", 3) == 0) {
            ObjTexCoord* t = (ObjTexCoord*)dynarray_push(texcoords);
            if (!t) { fclose(f); return 0; }
            sscanf(line + 3, "%f %f", &t->u, &t->v);
        } else if (strncmp(line, "vn ", 3) == 0) {
            ObjNormal* n = (ObjNormal*)dynarray_push(normals);
            if (!n) { fclose(f); return 0; }
            sscanf(line + 3, "%f %f %f", &n->x, &n->y, &n->z);
        } else if (strncmp(line, "f ", 2) == 0) {
            ObjFaceVertex fv[3];
            int parsed = sscanf(line + 2, "%d/%d/%d %d/%d/%d %d/%d/%d",
                                &fv[0].v, &fv[0].vt, &fv[0].vn,
                                &fv[1].v, &fv[1].vt, &fv[1].vn,
                                &fv[2].v, &fv[2].vt, &fv[2].vn);
            if (parsed == 9) {
                for (int i = 0; i < 3; i++) {
                    ObjFaceVertex* fp = (ObjFaceVertex*)dynarray_push(faces);
                    if (!fp) { fclose(f); return 0; }
                    *fp = fv[i];
                }
            }
        }
    }

    fclose(f);
    return 1;
}

ObjModel* obj_load(const char* path, const char* texture_path) {
    DynArray verts = dynarray_create(sizeof(ObjVertex), 1024);
    DynArray texcoords = dynarray_create(sizeof(ObjTexCoord), 1024);
    DynArray normals = dynarray_create(sizeof(ObjNormal), 1024);
    DynArray faces = dynarray_create(sizeof(ObjFaceVertex), 2048);

    if (!obj_parse_file(path, &verts, &texcoords, &normals, &faces)) {
        dynarray_free(&verts);
        dynarray_free(&texcoords);
        dynarray_free(&normals);
        dynarray_free(&faces);
        return NULL;
    }

    int face_count = faces.count;

    if (face_count == 0) {
        log_error("OBJ: No faces found in %s", path);
        dynarray_free(&verts);
        dynarray_free(&texcoords);
        dynarray_free(&normals);
        dynarray_free(&faces);
        return NULL;
    }

    Vertex* vertices = malloc(face_count * sizeof(Vertex));
    GLuint* indices = malloc(face_count * sizeof(GLuint));

    for (int i = 0; i < face_count; i++) {
        ObjFaceVertex* fv = (ObjFaceVertex*)dynarray_get(&faces, i);
        ObjVertex* v = (ObjVertex*)dynarray_get(&verts, fv->v - 1);

        ObjTexCoord t = {0.0f, 0.0f};
        if (fv->vt > 0 && fv->vt <= texcoords.count)
            t = *(ObjTexCoord*)dynarray_get(&texcoords, fv->vt - 1);

        ObjNormal n = {0.0f, 0.0f, 0.0f};
        if (fv->vn > 0 && fv->vn <= normals.count)
            n = *(ObjNormal*)dynarray_get(&normals, fv->vn - 1);

        vertices[i] = (Vertex){v->x, v->y, v->z, t.u, t.v, n.x, n.y, n.z};
        indices[i] = (GLuint)i;
    }

    Mesh* mesh = mesh_create(vertices, indices, face_count, face_count);

    ObjModel* model = malloc(sizeof(ObjModel));
    model->mesh = mesh;
    model->texture = texture_path ? texture_create_file(texture_path) : NULL;
    model->path = strdup(path);

    free(vertices);
    free(indices);
    dynarray_free(&verts);
    dynarray_free(&texcoords);
    dynarray_free(&normals);
    dynarray_free(&faces);

    log_info("OBJ loaded: %s (%d faces)", path, face_count / 3);
    return model;
}

void obj_render(ObjModel* model) {
    if (model && model->mesh) mesh_render(model->mesh);
}

void obj_destroy(ObjModel* model) {
    if (model) {
        mesh_destroy(model->mesh);
        if (model->texture) texture_destroy(model->texture);
        free(model->path);
        free(model);
    }
}
